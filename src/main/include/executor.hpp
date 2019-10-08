/* Author Jet Jacobs
 * Date:  10/5/19
 * This file holds all the declarations for the processing methods required for
 * managing commands passed to it. This may include parsing, pipeing, and forking
 * to proccess commands.
*/

#include <string>
#include <vector>

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

/* 
 * This is just a simple data structure to store process specific info.
*/
class Process
{
private:
    pid_t pid;
    std::vector<std::string> command;

public:
    Process(std::vector<std::string> command);
    pid_t getPID();
    void setPID(pid_t passedPID);
};

/* 
 * This is the actual class that handles the processing.
*/
class Executor
{
private:
    int pipeOne[2];
    int pipeTwo[2];

    std::vector<Process> processes;
    std::vector<int[2]> pipes;
    std::vector<std::string> customCommands;
    void execute(std::vector<std::string> command);

public:
    Executor();

    int handleExec(std::string command);
    void handlePipes(std::vector<std::string> command);
    void initPipes();
};

#endif