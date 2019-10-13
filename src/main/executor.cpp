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
const int PATH_MAX = 256;

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
        if (command[i] == "<")
        {
            if (i + 1 < command.size())
            {
                freopen((char *)command[i + 1].c_str(), "r", stdin); //redirect in
                i++;
            }
        }
        else if (command[i] == ">>")
        { //if there is another parameter consume it as file input
            if (i + 1 < command.size())
            {
                freopen((char *)command[i + 1].c_str(), "a", stdout); //redirect out
                freopen((char *)command[i + 1].c_str(), "a", stderr);
                i++;
            }
        }
        else if (command[i] == ">")
        { //if there is another parameter consume it as file input
            if (i + 1 < command.size())
            {
                freopen((char *)command[i + 1].c_str(), "w", stdout); //redirect out
                freopen((char *)command[i + 1].c_str(), "w", stderr);
                i++;
            }
        }
        else if (command[i] == "clr" && i == 0) //if the command is clr so we can ls clr if needed
            argv.push_back((char *)customCommands[0].c_str());
        else if (command[i] == "environ" && i == 0)
            argv.push_back((char *)customCommands[1].c_str());
        else if (command[i] == "dir" && i == 0)
        {
            if (command.size() < 2)
            {
                std::cout << "Invalid format: dir requires at least one argument";
                exit(0);
            }
            argv.push_back((char *)customCommands[2].c_str());
        }
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
    std::vector<std::string> *arguments = parseCommands(command);
    if (pipeCommands->size() != 1) //Check if it is a piped command
    {
        if ((*arguments)[arguments->size() - 1] == "&") //If the last arg is &
        {
            pid_t childPID = fork(); //Create a fork, and don't wait for child in parent
            if (childPID == 0)
            {
                handlePipes(*pipeCommands);
                exit(0); //child exits once process was done.
            }
            else if (childPID < 0)
                std::cout << "Issue forking";
        }
        else
            handlePipes(*pipeCommands); //Defaults to waiting for the process
    }
    else //Run as unpiped command
    {
        if ((*arguments)[0] == "quit")
            exit(0);
        else if ((*arguments)[0] == "pause")
        {
            std::string temp = *new std::string();
            std::cout << "Press enter to continue...";
            getline(std::cin, temp); // waits for input
        }
        else if ((*arguments)[0] == "help")
        {
            std::string newCmd = "man --help | more" + command.substr(4); //the whole command
            std::vector<std::string> *newPipeCommands = tokenizePipeCommands(newCmd);
            handlePipes(*newPipeCommands); //Process new piped command
        }
        else if ((*arguments)[0] == "cd")
        {
            char buff[PATH_MAX];
            std::string directory = "";
            if (arguments->size() > 1)
            {
                if ((*arguments)[1] == ">>" || (*arguments)[1] == ">") //handle redirect
                {
                    // This is to list current dir, but support >> and > redirects
                    pid_t childPID = fork(); //Create a fork, and don't wait for child in parent
                    if (childPID == 0)
                    {
                        std::string temp = "pwd";
                        (*arguments)[0] = temp;
                        execute(*arguments);
                        exit(0); //child exits once process was done.
                    }
                    else if (childPID < 0)
                        std::cout << "Issue forking";
                    else
                        waitpid(childPID, NULL, 0);
                }
                else if ((*arguments)[1] != ";" || (*arguments)[1] != "<")
                {
                    directory = (*arguments)[1]; //Get everything after "cd "
                    if (chdir(directory.c_str()) == -1)
                        std::cout << "Error Finding the directory.\n";
                }
            }
            else if (directory == "") // if no arguments were passed default to writing out dir
            {
                getcwd(buff, 256);
                std::cout << buff;
            }
        }
        else
        {
            pid_t childPID = fork();
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
}

/*
 * This handles branching and piping to execute as closely as I could to
 * the behaviour to my linux shell. Defaults waiting for process.
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
    std::vector<std::string> *arguments;

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
        arguments = parseCommands(command[i]);

        if (childPID == 0)
        {
            if (i == 0) //If first read in from stdin and write to the first pipe
            {
                dup2(pipes[i].ends[WRITE_END], STDOUT_FILENO);
                close(pipes[i].ends[READ_END]);
                close(pipes[i].ends[WRITE_END]);
                //initPipes(STDIN_FILENO, pipes[0].ends[WRITE_END]);
            }
            else if (i != numCmds - 1) //If a middle item read from previous pipe
            {                          // and write to write end of current pipe.
                initPipes(pipes[i - 1].ends[READ_END], pipes[i].ends[WRITE_END]);
            }
            else //If at the end read from previous pipes
            {    //READ_END, and write to STDOUT_FILENO
                dup2(pipes[i - 1].ends[READ_END], STDIN_FILENO);
                close(pipes[i - 1].ends[READ_END]);
                close(pipes[i - 1].ends[WRITE_END]);
                //initPipes(pipes[i - 1].ends[READ_END], STDOUT_FILENO);
            }

            if (arguments->back() == "&") //Small handler for background procs
                arguments->pop_back();
            execute(*arguments);
            std::cout << "Exec failed";
        }
        else if (childPID < 0)
        {
            std::cout << "Error forking";
            exit(1);
        }
    }
    for (int i = 0; i < pipes.size(); i++)
    {
        close(pipes[i].ends[READ_END]);
        close(pipes[i].ends[WRITE_END]);
    }
    pipes.empty();
    waitpid(childPID, NULL, 0);
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