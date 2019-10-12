/* Author Jet Jacobs
 * Date:  10/5/19
 * This file holds all the implementations for the processing methods required for
 * managing commands passed to it. This includes pipeing, and forking
 * to proccess commands.
 */
#include "include/executor.hpp"
#include "include/cmdParser.hpp"

#define READ_END 0
#define WRITE_END 1

/*
 * Constructor for the executor, doesn't require any parameters
 * 
 * Parameters: None
 * Returns: An instance of an Executor
 */
Executor::Executor()
{
    //These are used as fixed replacements in the execute function.
    customCommands = {"clear", "printenv", "ls"};
}

/*
 * This is the method that handles execution by taking the vector
 * and formatting it to a useable format for an exec function.
 * 
 * Parameters: 
 *      command: is a vector of strings in which each string
 *      represents a parameter to the command to be executed.
 * Returns: Nothing
 */
void Executor::execute(std::vector<std::string> command)
{
    std::vector<char *> argv = std::vector<char *>();
    //This creates a argument vector that can be accepted by execvp
    for (int i = 0; i < command.size(); i++)
    {
        //If there is a given command load another instead. Effectively an alias.
        //The rest is formatting to be passed to execvp.
        if (command[i] == "clr")
            argv.push_back((char *)customCommands[0].c_str());
        else if (command[i] == "environ")
            argv.push_back((char *)customCommands[1].c_str());
        else if (command[i] == "dir")
            argv.push_back((char *)customCommands[2].c_str());
        else
            argv.push_back((char *)command[i].c_str());
    }
    argv.push_back(NULL);

    execvp(argv[0], &argv[0]);
    std::cout << "This appears to be an unknown command";
    exit(0);
}

/*
 * This handles the control flow of how a given command is processed.
 * It does this by checking for pipes and other symbols.
 * 
 * Parameters:
 *      Command: This is a string already parsed for ; seperators
 * Returns: 0 on sucsessfull run when returning to myshell main
*/
int Executor::handleExec(std::string command)
{
    std::vector<std::string> *pipeCommands = tokenizePipeCommands(command);
    if (pipeCommands->size() != 1) //Check if it is a piped command
        handlePipes(*pipeCommands);
    else //Run as unpiped command
    {
        pid_t childPID = fork();
        std::vector<std::string> *arguments = parseCommands(command);
        if (childPID == 0)
        {
            if (arguments->back() == "&") //Handler for background running
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

/*
 * This handles branching and piping to execute as closely as I could to
 * the behaviour to my linux shell.
 * 
 * Parameters:
 *      Command: This is a string already parsed for ; and | seperators
 * Returns: Nothing
 */
void Executor::handlePipes(std::vector<std::string> command)
{
    //Declaring variables
    pid_t childPID;
    int numCmds = command.size();
    pipes = *new std::vector<pipeStruct>();

    for (int i = 0; i < numCmds; i++)
    {
        //Check if another pipe is needed and add one if so
        if (i < numCmds - 1)
        {
            pipeStruct p = *new pipeStruct();
            if (pipe(p.ends) != 0)
                std::cout << "Pipe failed.";
            pipes.push_back(p);
        }

        childPID = fork();
        std::vector<std::string> *arguments = parseCommands(command[i]);

        if (childPID == 0)
        {
            if (i == 0) //If first read in from stdin and write to the first pipe
            {
                close(pipes[0].ends[READ_END]);
                initPipes(STDIN_FILENO, pipes[0].ends[WRITE_END]);
                close(pipes[0].ends[WRITE_END]);
            }
            else if (i != numCmds - 1) //If a middle item read from previous pipe
            {                          // and write to write end of current pipe.
                initPipes(pipes[i - 1].ends[READ_END], pipes[i].ends[WRITE_END]);
            }
            else //If at the end read from previous pipes
            {    //READ_END, and write to STDOUT_FILENO
                close(pipes[i - 1].ends[WRITE_END]);
                initPipes(pipes[i - 1].ends[READ_END], STDOUT_FILENO);
                close(pipes[i - 1].ends[READ_END]);
            }

            if (arguments->back() == "&") //Small handler for background procs
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

/*
 * Disclamer this is not yet implemented. This is to handle the redirect logic. It will ideally be added in the
 * handleExec() method. 
 * 
 * Parameters:
 *      Command: This is a string already parsed for ; seperators
 * Returns: Nothing
 */
void Executor::handleRedirect(std::string command)
{
    std::vector<char *> argv = std::vector<char *>();
    std::vector<std::string> arguments = *parseCommands(command);

    for (int i = 0; i < arguments.size(); i++)
    {
        if (arguments[i] == ">>")
        {
        }
        else if (arguments[i] == ">")
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
 * I haven't yet fully implemented this part of the project.
 * The idea is that it works similarly to how initPipes works, but
 * with files passed via commandline.
 * 
 * Parameters:
 *      inFile: is the param to replace STDIN_FILENO
 *      outFile: is the param to replace STDOUT_FILENO
 * Returns: Nothing
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
 * This is a simple helper method to make piping easier.
 * It works by updating the the in and out files to the given parameters.
 * No changes occur if they are the same value.
 * 
 * Parameters:
 *      readEnd: is the param to replace STDIN_FILENO
 *      writeEnd: is the param to replace STDOUT_FILENO
 * Returns: Nothing
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