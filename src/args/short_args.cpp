#include "short_args.h"
#include "debug.h"
#include "options.h"
#include "print_help.h"

#include <string_view>
#include <vector>

void short_args(Basic_data& data, vector<string>& arg_list, string_view arg) {

    Args_data args_data = {
        .data = data,
        .arg_list = arg_list,
        .arg = arg,
    };

    for (size_t i = 1; i < arg.size(); i++) {
        if (arg[i] == 'h') {
            print_help();
            exit(0);
        }
        if (arg[i] == 'l') {
            Opts::limit(args_data);
            continue;
        }
        if (arg[i] == 'i') {
            data.p_data->index = true;
            dbg("setting indexed true");
            continue;
        }
        if (arg[i] == 'q') {
            Opts::quantity(args_data);
            continue;
        }
        if (arg[i] == 'd') {
            Opts::dirs(args_data);
            continue;
        }
        if (arg[i] == 'f') {
            Opts::file(args_data);
            continue;
        }
        if (arg[i] == 'a') {
            break;
        }
        if (arg[i] == 'v') {
            set_verbose(true);
            dbg("setting verbose mode : done");
            continue;
        }
        if (arg[i] == 'V') {
            std::println("random: {}", APP_VERSION);
            exit(0);
        }
    }
}
