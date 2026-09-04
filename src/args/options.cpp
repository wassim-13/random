#include "options.h"
#include "args/debug.h"
#include "core/utils.h"

namespace Opts {

void quantity(Args_data& args_data) {
    if (!args_data.arg_list.empty()) {
        string_view str = args_data.arg_list[args_data.arg_list.size() - 1];
        dbg("setting quantity as {}", str);
        args_data.data.p_data->quantity = get_number(str, -1);
        args_data.arg_list.pop_back();
        dbg("arg_list now {}", args_data.arg_list);
    }
}

void dirs(Args_data& args_data) {
    args_data.data.a_data->dirs.append_range(args_data.arg_list);
    dbg("appending {} to dirs and clearing arg_list: done", args_data.arg_list);
    args_data.arg_list.clear();
}

void file(Args_data& args_data) {
    args_data.data.a_data->files.append_range(args_data.arg_list);
    dbg("appending {} to files and clearing arg_list: done",
        args_data.arg_list);
    args_data.arg_list.clear();
}

void limit(Args_data& args_data) {
    if (!args_data.arg_list.empty()) {
        string_view str = args_data.arg_list[args_data.arg_list.size() - 1];
        dbg("setting quantity as {}", str);
        args_data.data.p_data->limit = get_number(str, 8);
        args_data.arg_list.pop_back();
        dbg("arg_list now {}", args_data.arg_list);
    }
}

} // namespace Opts
