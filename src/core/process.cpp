#include "process.h"
#include "args/debug.h"

#include <filesystem>
#include <fstream>
#include <print>
#include <vector>

namespace fs = std::filesystem;

using namespace std;

vector<string> process::dirs(vector<string>& dirs) {
    vector<string> file_paths;

    for (auto& dir : dirs) {
        fs::path tdir = dir;

        if (!fs::exists(tdir) || !fs::is_directory(tdir)) {
            println(stderr, "Error: Provided path is not a valid directory");
            continue;
        }

        std::error_code ec;
        auto iter_options = fs::directory_options::skip_permission_denied;

        for (fs::recursive_directory_iterator it(tdir, iter_options, ec), end;
             it != end; it.increment(ec)) {
            if (ec) {
                println(stderr, "Warning: Cannot access {} ({})",
                        it->path().string(), ec.message());
                continue;
            }

            if (fs::is_regular_file(it->status())) {
                fs::path relative = fs::relative(it->path(), tdir);
                file_paths.push_back(relative.string());
            }
        }
    }
    return file_paths;
}
std::vector<std::string> process::files(std::vector<std::string>& files) {

    println("what a beutiful day");

    vector<string> lines;

    for (auto& fil : files) {

        fs::path filepath{fil};

        std::error_code ec;

        if (!fs::exists(filepath, ec)) {
            println(stderr, "File does not exist: {}", fil);
            continue;
        }

        if (!fs::is_regular_file(filepath, ec)) {
            println(stderr, "Not a regular file: {}", fil);
            continue;
        }

        std::ifstream file(filepath);

        if (!file.is_open()) {
            println(stderr, "Failed to open file: {}", fil);
        }

        std::string line;

        while (std::getline(file, line)) {
            lines.push_back(std::move(line));
        }

        if (file.bad()) {
            println(stderr, "I/O read failure on: {}", fil);
            continue;
        }
    }

    return lines;
}
// namespace process
