#include <iostream>

#include "../main/include/cmdParser.hpp"
#include "../main/include/executor.hpp"

const std::string divider = "--------------------------------------";
int main()
{
    std::string testString = "ls -l";
    std::string testString1 = "cat src/main/main.cpp | more";
    std::string testString2 = "ls -l ; ls";
    std::string multiPipe = "ls | head -4 | head -1";
    Executor executor = Executor();

    /*
     *
     * Tests the handle pipes functionality
     * 
    */
    std::vector<std::string> parserOutput = *tokenizePipeCommands(testString1);
    std::cout << parserOutput[0] << "|" << parserOutput[1] << "\n";
    std::cout << divider << "\n";
    executor.handlePipes(parserOutput);

    parserOutput = *tokenizePipeCommands(multiPipe);
    std::cout << parserOutput[0] << "|" << parserOutput[1] << "|" << parserOutput[2] << "\n";
    std::cout << divider << "\n";
    executor.handlePipes(parserOutput);

    return 0;
}