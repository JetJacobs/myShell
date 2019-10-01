#include <vector>
#include <string>

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
    void setInput(std::string inputFile);
    void setOutput(std::string outputFile);
};

void exeForground(Process passedProcess);
void exeBackground(Process passedProcess);

#endif