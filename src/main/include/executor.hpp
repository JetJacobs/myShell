/* Author Jet Jacobs
 * Date:  10/5/19
 * This file holds all the declarations for the processing methods required for
 * managing commands passed to it. This includes pipeing, and forking
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
    //This is a vector of storing arguments to replace those like "clr"
    std::vector<std::string> customCommands;
    void execute(std::vector<std::string> command);
    void initPipes(int readEnd, int writeEnd);
    void initRedirect(std::string inFile, std::string outFile);

public:
    Executor();
    int handleExec(std::string command);
    void handlePipes(std::vector<std::string> command);
    void handleRedirect(std::string command);
};

#endif