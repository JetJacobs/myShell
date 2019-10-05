#include <string>
#include <vector>

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

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

class Executor
{
private:
    std::vector<Process> processes;
    std::vector<int[2]> pipes;

public:
    Executor();

    int exeForground(std::vector<std::string> command);
    void exeBackground(std::vector<std::string> command);
    void execute(std::vector<std::string> command);
    void handlePipes();
};

#endif