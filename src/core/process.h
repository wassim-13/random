#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

namespace process {

std::vector<std::string> dirs(std::vector<std::string>& dirs);
std::vector<std::string> files(std::vector<std::string>& dirs);

} // namespace process

#endif
