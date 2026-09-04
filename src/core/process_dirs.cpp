#include "process.h"

#include <chrono>
#include <condition_variable>
#include <cstring>
#include <deque>
#include <fcntl.h>
#include <filesystem>
#include <mutex>
#include <print>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace fs = std::filesystem;

using namespace std;
struct DirTask {
    fs::path dir_path;
    size_t root_prefix_len;
};

class WorkQueue {
  public:
    void push(DirTask task) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            tasks_.push_back(std::move(task));
        }
        cv_.notify_one();
    }

    void push_batch(std::vector<DirTask>& batch) {
        if (batch.empty())
            return;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            tasks_.insert(tasks_.end(), std::make_move_iterator(batch.begin()),
                          std::make_move_iterator(batch.end()));
        }
        cv_.notify_all();
    }

    bool pop(DirTask& task) {
        std::unique_lock<std::mutex> lock(mtx_);
        while (tasks_.empty()) {
            if (active_workers_ == 0) {
                return false;
            }
            cv_.wait(lock);
        }
        task = std::move(tasks_.front());
        tasks_.pop_front();
        ++active_workers_;
        return true;
    }

    void finish_work() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            --active_workers_;
        }
        cv_.notify_all();
    }

  private:
    std::mutex mtx_;
    std::condition_variable cv_;
    std::deque<DirTask> tasks_;
    size_t active_workers_ = 0;
};

vector<string> process::dirs(vector<string>& dirs) {

    if (dirs.empty())
        return {};

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
        num_threads = 4;

    WorkQueue queue;

    for (const auto& raw_dir : dirs) {
        fs::path p(raw_dir);
        std::error_code ec;
        if (fs::is_directory(p, ec)) {
            std::string base = p.string();
            size_t prefix_len = base.size();
            if (!base.empty() && base.back() != '/' && base.back() != '\\') {
                prefix_len += 1;
            }
            queue.push({std::move(p), prefix_len});
        }
    }

    std::vector<std::vector<std::string>> local_buffers(num_threads);
    for (auto& buf : local_buffers) {
        buf.reserve(8192);
    }

    std::vector<std::thread> workers;
    workers.reserve(num_threads);

    for (unsigned int t = 0; t < num_threads; ++t) {
        workers.emplace_back([&, t]() {
            auto& my_buffer = local_buffers[t];
            std::vector<DirTask> local_subdirs;
            local_subdirs.reserve(64);

            DirTask current_task;
            while (queue.pop(current_task)) {
                std::error_code ec;
                auto options = fs::directory_options::skip_permission_denied;
                fs::directory_iterator it(current_task.dir_path, options, ec);
                fs::directory_iterator end;

                while (it != end && !ec) {
                    if (it->is_symlink(ec)) {
                    } else if (it->is_directory(ec)) {
                        local_subdirs.push_back(
                            {it->path(), current_task.root_prefix_len});
                    } else if (it->is_regular_file(ec)) {
                        const std::string& full_path = it->path().native();
                        if (full_path.size() > current_task.root_prefix_len) {
                            my_buffer.emplace_back(
                                full_path.substr(current_task.root_prefix_len));
                        } else {
                            my_buffer.emplace_back(full_path);
                        }
                    }

                    it.increment(ec);
                }

                if (!local_subdirs.empty()) {
                    queue.push_batch(local_subdirs);
                    local_subdirs.clear();
                }

                queue.finish_work();
            }
        });
    }

    for (auto& worker : workers) {
        if (worker.joinable())
            worker.join();
    }

    size_t total_files = 0;
    for (const auto& buf : local_buffers) {
        total_files += buf.size();
    }

    std::vector<std::string> file_paths;
    file_paths.reserve(total_files);

    for (auto& buf : local_buffers) {
        file_paths.insert(file_paths.end(),
                          std::make_move_iterator(buf.begin()),
                          std::make_move_iterator(buf.end()));
    }

    return file_paths;
}
