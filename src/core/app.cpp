#include "app.h"

#include "args/debug.h"
#include "process.h"

#include <algorithm>
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

    if (!app_data->dirs.empty()) {
        app_data->extras.append_range(process::dirs(app_data->dirs));
    }

    if (!app_data->files.empty()) {
        app_data->extras.append_range(process::files(app_data->files));
    }

    random_device rd;
    mt19937 gen(rd());

    shuffle(app_data->extras.begin(), app_data->extras.end(), gen);

    for (auto& str : app_data->extras) {
        println("{}", str);
    }
}
