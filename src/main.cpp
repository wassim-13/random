/*
#include "run.h"
#include "treat_args.h"

using namespace std;

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
*/

#include "stack.h"

int main()
{
    Stack<int>* stack = new Stack<int>;
    stack->push(5);
    stack->push(3);
    stack->push(9);
    stack->push(7);

    stack->print();

    std::cout << "popint element\n";

    stack->pop();
    stack->print();
}
