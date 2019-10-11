/* Author Jet Jacobs
 * Date:  10/5/19
 * This file holds all the declarations for the processing methods required for
 * managing commands passed to it. This may include parsing, pipeing, and forking
 * to proccess commands.
*/

#include <iostream>
#include <string>
#include <vector>

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

struct pipeStruct
{
    int ends[2];
};

/* 
 * This is the actual class that handles the processing.
*/
class Executor
{
private:
    std::vector<pipeStruct> pipes;
    std::vector<std::string> customCommands;
    void execute(std::vector<std::string> command);
    void startProcess(int readEnd, int writeEnd);

public:
    Executor();

    int handleExec(std::string command);
    void handlePipes(std::vector<std::string> command);
    void initPipes();
};

#endif