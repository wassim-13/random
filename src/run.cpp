#include "run.h"

#include <iostream>
#include <random>

void run(App& app, vector<string> args)
{
    random_device rd;
    mt19937 gen(rd());

    if (app.mode.input)
    {
        string str;

        cout << "enter words : ";
        flush(cout);
        getline(cin, str);

        args = split_whitespace(str);
    }

    if (app.mode.normal)
    {
        shuffle(args.begin(), args.end(), gen);
    }

    app.printMode(args);
}
