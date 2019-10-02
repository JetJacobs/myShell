#include "cmdParser.hpp"
#include "executor.hpp"

int main()
{
    std::string testString = "ls";
    std::vector<std::string> *commands = tokenizeInput(testString);
    exeForground(*commands);
    return 0;
}