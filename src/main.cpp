#include <algorithm>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <print>
#include <random>
#include <string>
#include <vector>

using namespace std;

vector<string> split_whitespace(const string& str)
{
    vector<string> vec_s;
    string temp;

    for (size_t j = 0; j <= str.size(); j++)
    {
        if (str[j] == ' ' || str[j] == '\n' || str[j] == '\0')
        {
            vec_s.push_back(temp);
            temp.clear();
            continue;
        }
        temp.push_back(str[j]);
    }
    return vec_s;
}

struct Mode
{
    bool normal;
    bool input;
    bool hours;
};

struct Data
{
    int hours;
};

enum PMode
{
    normal,
    hours,
};
class App
{
  public:
    Mode mode;
    Data data;

    void printMode(vector<string> args)
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

                while (left > 0)
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

  private:
    PMode which_pmode()
    {
        if (this->mode.hours == true)
        {
            return PMode::hours;
        }
        return PMode::normal;
    }
};

App treat_args(vector<string>& args)
{
    Mode mode(true, false, false);
    Data data;

    if (args.empty())
    {
        mode.input = true;
    }

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
            std::erase(args, a);
            continue;
        }
        if (n_hours)
        {
            data.hours = stoi(a);
            n_hours = false;
        }
    }

    return App{mode, data};
}

void run(App& app, vector<string> args)
{
    random_device rd;
    mt19937 gen(rd());

    if (app.mode.input)
    {
        string str;

        print("enter words : ");
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

int main(int argc, char* argv[])
{
    vector<string> args;

    for (int i = 1; i < argc; i++)
    {
        args.push_back(string(argv[i]));
    }

    App app = treat_args(args);

    run(app, args);

    return 0;
}
