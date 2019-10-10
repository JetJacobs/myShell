#include <iostream>

#include "../main/include/cmdParser.hpp"
#include "../main/include/executor.hpp"

const std::string divider = "--------------------------------------";
int main()
{
    std::string testString = "ls -l";
    //std::string testString1 = "cat src/main/executor.cpp | more ";
    std::string testString1 = "ls -l";
    std::string testString2 = "ls -l ; ls";
    std::string multiPipe = "ls | head -4 | head -1";
    Executor executor = Executor();

    /* 
     * This is the start of parsing tests nothing too fancy.
    */
    std::cout << "\nParse tests\n";
    std::cout << divider << '\n';

    std::cout << "Parsing string with no pipe\n";
    std::vector<std::string> parserOutput = *tokenizePipeCommands(testString);
    std::cout << "\tExpect: 1\tActual: " << parserOutput.size() << "\n";

    std::cout << "Parsing string with pipe\n";
    parserOutput = *tokenizePipeCommands(testString1);
    std::cout << "\tExpect: 2\tActual: " << parserOutput.size() << "\n";

    std::cout << "Parsing string by whitespace\n";
    parserOutput = *parseCommands(testString);
    std::cout << "\tExpect: 2\tActual: " << parserOutput.size() << "\n";

    std::cout << "Parsing by whitespace with pipe\n";
    parserOutput = *parseCommands(testString1);
    std::cout << "\tExpect: 4\tActual: " << parserOutput.size() << "\n";

    std::cout << "Parsing string by semicolon pipes or semicolon\n";
    parserOutput = *tokenizeInputToCommands(testString);
    std::cout << "\tExpect: 1\tActual: " << parserOutput.size() << "\n";

    std::cout << "Parsing by semicolon no semicolon\n";
    parserOutput = *tokenizeInputToCommands(testString1);
    std::cout << "\tExpect: 1\tActual: " << parserOutput.size() << "\n";

    std::cout << "Parsing by semicolon with semicolon\n";
    parserOutput = *tokenizeInputToCommands(testString2);
    std::cout << "\tExpect: 2\tActual: " << parserOutput.size() << "\n";

    parserOutput = *tokenizePipeCommands(testString1);
    std::cout << parserOutput[0] << "\n";
    std::cout << divider << "\n";
    executor.handlePipes(parserOutput);

    parserOutput = *tokenizePipeCommands(multiPipe);
    std::cout << parserOutput[0] << "|" << parserOutput[1] << "|" << parserOutput[2] << "\n";
    std::cout << divider << "\n";
    executor.handlePipes(parserOutput);

    return 0;
}