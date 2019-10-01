#include "cmdParser.hpp"

int main()
{
    std::string testString = "This is | demo Input; duh";
    std::vector<std::string> *commands = tokenizeInput(testString);
    return 0;
}