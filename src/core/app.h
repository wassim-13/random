#ifndef APP_H
#define APP_H

#include "args/args.h"

using namespace std;

class App {
  public:
    App();

    void process_data();
    void run(vector<string>& args);

    ~App() = default;

  private:
    unique_ptr<App_data> app_data;
    unique_ptr<Print_data> print_data;

    vector<string> words;
};

#endif
