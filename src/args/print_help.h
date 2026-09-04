#ifndef PRINT_HELP_H
#define PRINT_HELP_H

#include <print>

constexpr void print_help() {
    constexpr auto reset = "\033[0m";
    constexpr auto green = "\033[32m";
    constexpr auto cyan = "\033[36m";
    constexpr auto bcyan = "\033[96m";

    std::println(
        R"({0}Usage:{3} {2}random{3} {1}[options] <arguments>{3}

    A cli for shuffeling words.

{0}Options:{3}
    {2}-a, --add{3}      {1}<words>{3}    Add extra words       
    {2}-d, --dir{3}      {1}<dirs>{3}     Specify a directory to shuffle files inside it
    {2}-f, --file{3}     {1}<file+o>{3}   Shuffle stuff in file
    {2}-h, --help{3}                Show this help message and exit
    {2}-i, --indexed{3}             Make an index to each output
    {2}-l, --limit{3}     {1}<num>{3}     Limit the output
    {2}-q, --quantity{3}  {1}<num>{3}     Specify the total number, give each a random number
    {2}-V, --version{3}             Display program version information
    {2}-v, --verbose{3}             Enable detailed debug logging
        {2}--one-line{3}            Output in one line

{0}Arguments:{3}
    {1}<dirs>{3} directories
    {1}<num>{3} a number
    {1}<file + o>{3} file path + what to shuffle? w: words l: lines

{0}Examples:{3}
    {0}random{3}  {2}-l{3} {1}3{3} word1 word2.. {2}-f{3} {1}file.txt w{3} {2}-d{3} {1}dir1 dir2..{3} {2}-a{3} word3 word4..

    )",
        green, cyan, bcyan, reset);
}
#endif
