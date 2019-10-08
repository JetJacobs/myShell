#include "include/executor.hpp"
#include "include/cmdParser.hpp"

Executor::Executor()
{
    customCommands = {"clear", "printenv"};
}

void Executor::execute(std::vector<std::string> command)
{
    std::vector<char *> argv = std::vector<char *>();
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

void Executor::handlePipes(std::vector<std::string> command)
{
}

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