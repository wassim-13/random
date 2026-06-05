#include "App.h"

#include <print>
#include <random>

void App::printMode(vector<string> args)
{
    PMode pmode = this->which_pmode();

    switch (pmode)
    {
        case PMode::normal:
        {
            for (const basic_string<char>& s : args)
            {
                print("{} ", s);
            }
            println();
            break;
        }
        case PMode::hours:
        {
            int left = data.hours;
            int i = 0;
            random_device rd;
            mt19937 gen(rd());

            while (left > 0 && i < static_cast<int>(args.size()))
            {
                uniform_int_distribution<int> dist(0, 2);
                int temp = 0;
                temp = 1 + dist(gen) % left;
                left -= temp;
                println("[{}] : {}", temp, args[i++]);
            }
            println();
        }
    }
}

PMode App::which_pmode()
{
    if (this->mode.hours == true)
    {
        return PMode::hours;
    }
    return PMode::normal;
}
