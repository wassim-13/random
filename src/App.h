#ifndef APP_H
#define APP_H

#include <string>
#include <vector>

using namespace std;

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

    void printMode(vector<string> args);

  private:
    PMode which_pmode();
};

#endif
