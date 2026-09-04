#include "core/utils.h"
#include "debug.h"
#include "input.h"
#include "long_args.h"

using namespace std;

Basic_data handle_args(vector<string>& args) {
    Basic_data data{
        .p_data = make_unique<Print_data>(Print_data{
            .list = true,
            .limit = -1,
            .quantity = -1,
            .index = false,
        }),
        .a_data = make_unique<App_data>(),
    };

    size_t args_size = args.size() - 1;

    vector<string> arg_list;

    dbg("args : {}, size : {}", args, args_size);

    for (size_t i = args_size; (int)i > -1; i--) {
        string& arg = args[i];

        if (arg.starts_with('-')) {

            if (arg.size() > 1 && arg[1] == '-') {
                long_args(data, arg_list, arg);
            } else {
                short_args(data, arg_list, arg);
            }

            if (!arg_list.empty()) {
                dbg("appending {} to extras", arg_list);
                data.a_data->extras.append_range(arg_list);
            }
            arg_list.clear();
        } else {
            arg_list.push_back(arg);
        }
    }

    if (!arg_list.empty()) {
        dbg("appending {} to extras", arg_list);
        data.a_data->extras.append_range(arg_list);
    }

    if (data.a_data->dirs.empty() && data.a_data->files.empty() &&
        data.a_data->extras.empty()) {
        string input = read_line("enter words : ");
        data.a_data->extras = split_whitespace(input);
    }

    return data;
}
