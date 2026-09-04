#ifndef OPTOINS_H
#define OPTIONS_H

#include "args/args.h"

struct Args_data {
    Basic_data& data;
    vector<string>& arg_list;
    string_view arg;
};

namespace Opts {

void quantity(Args_data& args_data);
void dirs(Args_data& args_data);
void file(Args_data& args_data);
void limit(Args_data& args_data);

} // namespace Opts

#endif
