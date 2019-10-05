#include "include/executor.hpp"

Process::Process(std::vector<std::string> passedcommand)
{
    command = passedcommand;
}
pid_t Process::getPID()
{
    return pid;
}
void Process::setPID(pid_t passedPID)
{
    pid = passedPID;
}

Executor::Executor() {}

int Executor::exeForground(std::vector<std::string> commands)
{
    pid_t childPID = fork();
    if (childPID == 0)
    {
        execute(commands);
    }
    else
    {
        waitpid(childPID, NULL, 0);
        return 0;
    }
}

void Executor::exeBackground(std::vector<std::string> commands)
{
}

void Executor::execute(std::vector<std::string> command)
{
    std::vector<char *> argv = std::vector<char *>();
    for (int i = 0; i < command.size(); i++)
    {
        argv.push_back((char *)command[i].c_str());
        printf("%s", argv[i]);
    }
    argv.push_back(NULL);
    execvp(argv[0], &argv[0]);

    printf("\n Exec failed\n");
    exit(0);
}

void Executor::handlePipes()
{
}