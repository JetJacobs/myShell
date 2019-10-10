#include "include/executor.hpp"
#include "include/cmdParser.hpp"
#include <iostream>

#define READ_END 0
#define WRITE_END 1

Executor::Executor()
{
    customCommands = {"clear", "printenv"};
}

void Executor::execute(std::vector<std::string> command)
{
    std::vector<char *> argv = std::vector<char *>();
    //This creates a argument vector that can be accepted by execvp
    for (int i = 0; i < command.size(); i++)
    {
        if (command[i] == "clr")
            argv.push_back((char *)customCommands[0].c_str());
        else if (command[i] == "environ")
            argv.push_back((char *)customCommands[1].c_str());
        else
            argv.push_back((char *)command[i].c_str());
    }
    argv.push_back(NULL);

    execvp(argv[0], &argv[0]);
    printf("\n Exec failed\n");
    exit(0);
}

int Executor::handleExec(std::string command)
{
    std::vector<std::string> *pipeCommands = tokenizePipeCommands(command);
    if (pipeCommands->size() != 1)
        handlePipes(*pipeCommands);
    else
    {
        pid_t childPID = fork();
        std::vector<std::string> *arguments = parseCommands(command);
        if (childPID == 0)
        {
            if (arguments->back() == "&")
                arguments->pop_back();
            execute(*arguments);
        }
        else
        {
            if (arguments->back() != "&")
                waitpid(childPID, NULL, 0);
            return 0;
        }
    }
}

void Executor::handlePipes(std::vector<std::string> command)
{
    //Pipe is made
    pid_t childPID;
    int numCmds = command.size();
    pipes = *new std::vector<pipeStruct>();
    pipeStruct p;

    for (int i = 0; i < numCmds; i++)
    {
        if (i < numCmds - 1)
        {
            p = *new pipeStruct();
            pipe(p.ends);
            pipes.push_back(p);
        }

        childPID = fork();
        std::vector<std::string> *arguments = parseCommands(command[i]);

        if (childPID == 0)
        {
            if (i == 0)
            {
                startProcess(STDIN_FILENO, pipes[i].ends[WRITE_END]);
            }
            else if (i < numCmds - 1)
            {
                startProcess(pipes[i - 1].ends[READ_END], pipes[i].ends[WRITE_END]);
            }
            else
            {
                startProcess(pipes[i - 1].ends[READ_END], STDIN_FILENO);
            }

            if (arguments->back() == "&")
                arguments->pop_back();
            execute(*arguments);
            std::cout << "Error Executing";
            exit(1);
        }
        else if (childPID < 0)
        {
            std::cout << "Error forking";
            exit(1);
        }
        else
        {
            waitpid(childPID, NULL, 0);
        }
    }
}

void Executor::startProcess(int readEnd, int writeEnd)
{
    if (writeEnd != STDOUT_FILENO) //if not the default write end reassign
    {
        dup2(writeEnd, STDOUT_FILENO);
        close(writeEnd);
    }
    if (readEnd != STDIN_FILENO) // if not the default read end reassign
    {
        dup2(readEnd, STDIN_FILENO);
        close(readEnd);
    }
}