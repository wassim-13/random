#include "app.h"

#include "args/debug.h"
#include "process.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <print>
#include <random>
#include <utility>

App::App() {
    app_data = make_unique<App_data>();
    print_data = make_unique<Print_data>(Print_data{
        .list = true,
        .limit = -1,
        .quantity = 8,
        .index = true,
    });
}

void App::run(vector<string>& args) {
    Basic_data data = handle_args(args);
    app_data = std::move(data.a_data);
    print_data = std::move(data.p_data);

    dbg("Handling Args : Done !");
    dbg("landed with :");
    dbg("App data :");
    dbg("-> dirs  {}", app_data->dirs);
    dbg("-> files {}", app_data->files);
    dbg("-> extra {}", app_data->extras);
    dbg("Print data :");
    dbg("-> limit    {}", print_data->limit);
    dbg("-> indexed  {}", print_data->index);
    dbg("-> quantity {}", print_data->quantity);
    dbg("-> list     {}", print_data->list);

    if (!app_data->dirs.empty()) {
        app_data->extras.append_range(process::dirs(app_data->dirs));
    }

    if (!app_data->files.empty()) {
        app_data->extras.append_range(process::files(app_data->files));
    }

    random_device rd;
    mt19937 gen(rd());

    shuffle(app_data->extras.begin(), app_data->extras.end(), gen);

    // TODO:
    // add a proper printings
    //

    // basic idea for -i and -l

    bool indexed = print_data->index;
    bool limit = false;
    char en = !print_data->list ? ' ' : '\n';

    if (print_data->limit >= 0) {
        limit = true;
    }

    int64_t line = 0;

    for (auto& str : app_data->extras) {
        if (limit && line > print_data->limit)
            break;

        if (indexed) {
            print("{} : {}{}", line, str, en);
        } else {
            print("{}{}", str, en);
        }
        line++;
    }
    println();
}
