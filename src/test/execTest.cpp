/* Author Jet Jacobs
 * Date:  10/5/19
 * This is just a very basic testing template for the pipe testing I fully expect to have this more fleshed out
 * by the due date.
 */

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
    std::string pipeRedirect = "ls | head -2 > test";
    Executor executor = Executor();

    /*
     *
     * Tests the handle pipes functionality
     * 
    */
    std::vector<std::string> *pipeTokens = tokenizePipeCommands(testString1);
    std::vector<std::string> *arguments = parseCommands(testString1);
    std::cout << pipeTokens[0] << "|" << pipeTokens[1] << "\n";
    std::cout << divider << "\n";
    executor.executePipes(pipeTokens, arguments);

    pipeTokens = tokenizePipeCommands(multiPipe);
    arguments = parseCommands(multiPipe);
    std::cout << pipeTokens[0] << "|" << pipeTokens[1] << "|" << pipeTokens[2] << "\n";
    std::cout << divider << "\n";
    executor.executePipes(pipeTokens, arguments);

    std::cout << pipeRedirect << "\n"
              << divider << "\n";
    executor.handleExec(pipeRedirect);

    return 0;
}