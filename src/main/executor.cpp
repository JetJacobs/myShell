#include "include/executor.hpp"
#include "include/cmdParser.hpp"

#define READ_END 0
#define WRITE_END 1

Executor::Executor()
{
    customCommands = {"clear", "printenv", "ls"};
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
        else if (command[i] == "dir")
            argv.push_back((char *)customCommands[2].c_str());
        else if (command[i] == "help")
        {
        }
        else
            argv.push_back((char *)command[i].c_str());
    }
    argv.push_back(NULL);

    execvp(argv[0], &argv[0]);
    std::cout << "This appears to be an unknown command";
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
        else if (childPID > 0)
        {
            if (arguments->back() != "&")
                waitpid(childPID, NULL, 0);
            return 0;
        }
        else
            std::cout << "Failed to fork.";
    }
}

void Executor::handlePipes(std::vector<std::string> command)
{
    //Declaring variables
    pid_t childPID;
    int numCmds = command.size();
    pipes = *new std::vector<pipeStruct>();
    pipeStruct p;

    for (int i = 0; i < numCmds; i++)
    {
        //Check if another pipe is needed and add one
        if (i < numCmds - 1)
        {
            p = *new pipeStruct();
            if (pipe(p.ends) != 0)
                std::cout << "Pipe failed.";
            pipes.push_back(p);
        }

        childPID = fork();
        std::vector<std::string> *arguments = parseCommands(command[i]);

        if (childPID == 0)
        {
            if (i == 0) //If first read in from stdin and write to the first pipe
                initPipes(STDIN_FILENO, pipes[i].ends[WRITE_END]);
            else if (i < numCmds - 1)
                initPipes(pipes[i - 1].ends[READ_END], pipes[i].ends[WRITE_END]);
            else
                initPipes(pipes[i - 1].ends[READ_END], STDOUT_FILENO);

            if (arguments->back() == "&")
                arguments->pop_back();
            execute(*arguments);
            std::cout << "Exec failed";
        }
        else if (childPID > 0)
        {
            waitpid(childPID, NULL, 0);
        }
        else
        {
            std::cout << "Error forking";
            exit(1);
        }
    }
}

void Executor::handleRedirect(std::string command)
{
    std::vector<char *> argv = std::vector<char *>();
    std::vector<std::string> arguments = *parseCommands(command);

    for (int i = 0; i < arguments.size(); i++)
    {
        if (arguments[i] == " >>")
        {
        }
        else if (arguments[i] == " >")
        {

        } //else if(input){}
        else if (arguments[i] == "clr")
            argv.push_back((char *)customCommands[0].c_str());
        else if (arguments[i] == "environ")
            argv.push_back((char *)customCommands[1].c_str());
        else if (arguments[i] == "dir")
            argv.push_back((char *)customCommands[2].c_str());
        else if (arguments[i] == "help")
        {
        }
        else
            argv.push_back((char *)arguments[i].c_str());
    }
}

/*
 * Just a general assignment, which allows me to kick off a process easily.
 * I could potentially move the fork in here, but wanted to keep that as part
 * of the handlePipe() function, as well as the redirect function.
*/
void Executor::initRedirect(std::string inFile, std::string outFile)
{
    if (!outFile.empty()) //If not the default write end reassign
    {
        freopen(outFile.c_str(), "w", stdout);
    }
    if (!inFile.empty()) //If not the default read end reassign
    {
        freopen(inFile.c_str(), "r", stdin);
    }
}

/*
 * Just a general assignment, which allows me to kick off a process easily.
 * I could potentially move the fork in here, but wanted to keep that as part
 * of the handlePipe() function, as well as the redirect function.
*/
void Executor::initPipes(int readEnd, int writeEnd)
{
    if (writeEnd != STDOUT_FILENO) //If not the default write end reassign
    {
        dup2(writeEnd, STDOUT_FILENO);
        close(writeEnd);
    }
    if (readEnd != STDIN_FILENO) //If not the default read end reassign
    {
        dup2(readEnd, STDIN_FILENO);
        close(readEnd);
    }
}