#include "core/utils.h"
#include "debug.h"
#include "options.h"
#include "print_help.h"

void long_args(Basic_data& data, vector<string>& arg_list, string_view arg) {

    Args_data args_data = {
        .data = data,
        .arg_list = arg_list,
        .arg = arg,
    };

    switch (hash_str(arg)) {
        case "--add"_h: {
            break;
        }
        case "--help"_h: {
            print_help();
            exit(0);
            break;
        }
        case "--verbose"_h: {
            set_verbose(true);
            dbg("--verbose : setting verbose mode : done");
            break;
        }
        case "--version"_h: {
            std::println("random: {}", APP_VERSION);
            exit(0);
        }

        case "--quantity"_h: {
            Opts::quantity(args_data);
            break;
        }
        case "--dir"_h: {
            Opts::dirs(args_data);
            break;
        }
        case "--file"_h: {
            Opts::file(args_data);
            break;
        }
        case "--oneline"_h: {
            data.p_data->list = false;
            break;
        }
        case "--indexed"_h: {
            data.p_data->index = true;
            dbg("setting indexed to true");
        }
        case "--limit"_h: {
            Opts::limit(args_data);
        }
    }
}
