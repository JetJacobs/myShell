#include "executor.hpp"

Process::Process(std::vector<std::string> command)
{
}
pid_t Process::getPID()
{
}
void Process::setPID(pid_t passedPID)
{
}

Executor::Executor(){};

void Executor::exeForground(std::vector<std::string> commands)
{
    pid_t childPID = fork();
    if (childPID == 0)
    {
        std::vector<char *> args;
        for (int i = 0; i < commands.size(); i++)
            args.push_back((char *)commands[i].c_str());
        args.push_back(NULL);
        execvp(args[0], &args[0]);
    }
    waitpid(childPID, NULL, 0);
}

void Executor::exeBackground(std::vector<std::string> commands)
{
}

void handlePipes()
{
}