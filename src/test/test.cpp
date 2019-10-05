#include <iostream>

#include "../main/include/cmdParser.hpp"
#include "../main/include/executor.hpp"

const std::string divider = "--------------------------------------";
int main()
{
    std::string testString = "ls -l";
    Executor executor = Executor();

    std::vector<std::string> *commands = parseCommands(testString);

    for (int i = 0; i < commands->size(); i++)
    {
        std::cout << &commands[i];
    }

    //Test execution method.
    executor.exeForground(*commands);

    std::cout << divider << '\n';

    return 0;
}