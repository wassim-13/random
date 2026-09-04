#include "debug.h"
#include "long_args.h"
#include "short_args.h"

#include <cstdlib>
#include <memory>

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

    return data;
}
