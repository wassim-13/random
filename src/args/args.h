#ifndef HANDLE_ARGS_H
#define HANDLE_ARGS_H

#include <memory>
#include <vector>

using namespace std;

struct Print_data {
    bool list;
    int64_t limit;
    int64_t quantity;
    bool index;
};
struct App_data {
    vector<string> dirs;
    vector<string> files;
    vector<string> extras;
};

struct Basic_data {
    unique_ptr<Print_data> p_data;
    unique_ptr<App_data> a_data;
};

Basic_data handle_args(vector<string>& args);

#endif
