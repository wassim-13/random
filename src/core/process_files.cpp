#include "process.h"

#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>

MappedFile::~MappedFile() {
    if (data && data != MAP_FAILED) {
        ::munmap(const_cast<char*>(data), size);
    }
}
namespace {

constexpr size_t CHUNK_SIZE = 8 * 1024 * 1024;

std::shared_ptr<MappedFile> map_file(const char* path) {
    int fd = ::open(path, O_RDONLY | O_NOATIME);
    if (fd == -1) {
        fd = ::open(path, O_RDONLY);
        if (fd == -1)
            return nullptr;
    }

    struct stat64 st;
    if (::fstat64(fd, &st) == -1 || st.st_size == 0 || !S_ISREG(st.st_mode)) {
        ::close(fd);
        return nullptr;
    }

    size_t size = static_cast<size_t>(st.st_size);
    void* addr = ::mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
    ::close(fd);

    if (addr == MAP_FAILED)
        return nullptr;

    ::madvise(addr, size, MADV_SEQUENTIAL | MADV_WILLNEED);

    auto mf = std::make_shared<MappedFile>();
    mf->data = static_cast<const char*>(addr);
    mf->size = size;
    return mf;
}

inline void parse_chunk_to_strings(const ChunkTask& task,
                                   std::vector<std::string>& out) {
    const char* base = task.file->data;
    const size_t total_size = task.file->size;

    const char* ptr = base + task.start_offset;
    const char* file_end = base + total_size;

    if (!task.is_first) {
        const char* first_nl = static_cast<const char*>(
            std::memchr(ptr, '\n', static_cast<size_t>(file_end - ptr)));
        if (!first_nl)
            return;
        ptr = first_nl + 1;
    }

    const char* scan_limit = base + task.end_offset;
    if (scan_limit > file_end)
        scan_limit = file_end;

    while (ptr < file_end) {
        if (!task.is_last && ptr >= scan_limit)
            break;

        const char* nl = static_cast<const char*>(
            std::memchr(ptr, '\n', static_cast<size_t>(file_end - ptr)));

        if (nl) {
            size_t len = static_cast<size_t>(nl - ptr);
            if (len > 0 && ptr[len - 1] == '\r')
                --len;
            out.emplace_back(ptr, len);
            ptr = nl + 1;

            if (!task.is_last && ptr >= scan_limit)
                break;
        } else {
            size_t len = static_cast<size_t>(file_end - ptr);
            if (len > 0 && ptr[len - 1] == '\r')
                --len;
            if (len > 0)
                out.emplace_back(ptr, len);
            break;
        }
    }
}

} // namespace

std::vector<std::string> process::files(const std::vector<std::string>& proc) {

    if (proc.empty())
        return {};

    std::vector<ChunkTask> tasks;
    tasks.reserve(proc.size() * 4);

    for (const auto& path : proc) {
        auto mf = map_file(path.c_str());
        if (!mf)
            continue;

        size_t file_size = mf->size;
        size_t offset = 0;

        while (offset < file_size) {
            size_t end = std::min(offset + CHUNK_SIZE, file_size);
            tasks.push_back({mf, offset, end, offset == 0, end == file_size});
            offset = end;
        }
    }

    if (tasks.empty())
        return {};

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
        num_threads = 4;
    num_threads = std::min<unsigned int>(
        num_threads, static_cast<unsigned int>(tasks.size()));

    std::vector<std::vector<std::string>> local_buffers(num_threads);
    for (auto& buf : local_buffers) {
        buf.reserve(32768);
    }

    std::atomic<size_t> task_idx{0};
    const size_t total_tasks = tasks.size();
    std::vector<std::thread> workers;
    workers.reserve(num_threads);

    for (unsigned int t = 0; t < num_threads; ++t) {
        workers.emplace_back([&, t]() {
            auto& my_buffer = local_buffers[t];

            while (true) {
                size_t idx = task_idx.fetch_add(1, std::memory_order_relaxed);
                if (idx >= total_tasks)
                    break;

                parse_chunk_to_strings(tasks[idx], my_buffer);
            }
        });
    }

    for (auto& worker : workers) {
        if (worker.joinable())
            worker.join();
    }

    size_t total_lines = 0;
    for (const auto& buf : local_buffers) {
        total_lines += buf.size();
    }

    std::vector<std::string> results;
    results.reserve(total_lines);

    for (auto& buf : local_buffers) {
        results.insert(results.end(), std::make_move_iterator(buf.begin()),
                       std::make_move_iterator(buf.end()));
    }

    return results;
}
