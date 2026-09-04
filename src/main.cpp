#include "core/app.h"

int main(int argc, char* argv[]) {

    App app;

    vector<string> args;

    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }

    app.run(args);
}
