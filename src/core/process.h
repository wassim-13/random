#ifndef PROCESS_H
#define PROCESS_H

#include <memory>
#include <string>
#include <vector>

struct MappedFile {
    const char* data = nullptr;
    size_t size = 0;

    ~MappedFile();
};

struct ChunkTask {
    std::shared_ptr<MappedFile> file;
    size_t start_offset;
    size_t end_offset;
    bool is_first;
    bool is_last;
};

struct ProcessedLines {
    std::vector<std::string_view> lines;
    std::vector<std::shared_ptr<MappedFile>> file_handles;
};

namespace process {

std::vector<std::string> dirs(std::vector<std::string>& dirs);
std::vector<std::string> files(const std::vector<std::string>& proc);
} // namespace process

#endif
