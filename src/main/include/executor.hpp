/* Author Jet Jacobs
 * Date:  10/5/19
 * This file holds all the declarations for the processing methods required for
 * managing commands passed to it. This includes pipeing, and forking
 * to proccess commands.
*/
#include <iostream>
#include <string>
#include <vector>

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

// Simple struct so I can store pipes in a vector.
struct pipeStruct
{
    int ends[2];
};

/* 
 * This is the actual class that handles the processing.
*/
class Executor
{
private:
    std::vector<pipeStruct> pipes;

    /*
     * This is the method that handles execution by taking the vector
     * and formatting it to a useable format for an exec function.
     * 
     * Parameters: 
     *      command: is a vector of strings in which each string
     *      represents a parameter to the command to be executed.
     * Returns: Nothing
     */
    void execute(std::vector<std::string> command);

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
    void initPipes(int readEnd, int writeEnd);

    /*
     * This handles branching and piping to execute as closely as I could to
     * the behaviour to my linux shell.
     * 
     * Parameters:
     *      Command: This is a string already parsed for ; and | seperators
     * Returns: 0 on sucsessfull run when returning to myshell main
     */
    void handlePipes(std::vector<std::string> command);

    void executeChangeDir(std::vector<std::string> *arguments);

public:
    /*
     * Constructor for the executor, doesn't require any parameters
     * 
     * Parameters: None
     * Returns: An instance of an Executor
     */
    Executor();

    /*
     * This handles the control flow of how a given command is processed.
     * It does this by checking for pipes and other symbols.
     * 
     * Parameters:
     *      Command: This is a string already parsed for ; seperators
     * Returns: 0 on sucsessfull run when returning to myshell main
     */
    int handleExec(std::string command);

    void executePipes(std::vector<std::string> *pipeCommands, std::vector<std::string> *arguments);
};

#endif