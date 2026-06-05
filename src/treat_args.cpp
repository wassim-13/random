#include "treat_args.h"

#include <iostream>

App treat_args(vector<string>& args)
{
    Mode mode(true, false, false);
    Data data;

    bool n_hours = false;

    for (auto& a : args)
    {
        if (a.starts_with('-'))
        {
            if (a.find('t'))
            {
                mode.hours = true;
                n_hours = true;
            }
            erase(args, a);
        }
        if (n_hours)
        {
            try
            {
                data.hours = std::stoi(a);
            }
            catch (...)
            {
                std::cerr << "Warning: expected a number.\n";
                data.hours = 8;
                n_hours = false;
                continue;
            }
            n_hours = false;
            erase(args, a);
        }
    }
    if (args.empty())
    {
        mode.input = true;
    }
    return App{mode, data};
}
