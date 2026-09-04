#include "app.h"

#include "args/debug.h"

#include <memory>
#include <print>
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

    for (auto& str : app_data->extras) {
        println("{}", str);
    }
}
