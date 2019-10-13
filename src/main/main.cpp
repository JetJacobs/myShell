/* Author Jet Jacobs
 * Date:  10/5/19
 * This is the main entry point for myshell. Most of the
 * complexity from my project was abstracted out of this to 
 * make it easy to read.
 * 
 * My shell can launch with either a commandline arg, or
 * by using the console input.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <string.h>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>

#include "include/cmdParser.hpp"
#include "include/executor.hpp"

const std::string BASHSYMBOL = "/myshell$ ";
const int PATH_MAX = 256;

std::string *input = new std::string();
std::vector<std::string> *tokens;
std::string PWD;
std::string currentDir;

void execLogic(Executor *executor);

int main(int argc, char **argv)
{
    bool exit = false;
    tokens = new std::vector<std::string>();
    Executor *executor = new Executor();

    char buff[PATH_MAX];
    getcwd(buff, PATH_MAX); //get the cwd note this doesn't track symbolic links
    currentDir = buff;
    std::string temp = "shell=" + currentDir + "/myshell";
    putenv((char *)temp.c_str()); //Set shell in env

    system("clear");
    if (argc == 1) //If no file was provided
    {
        do
        {
            std::cout << getcwd(buff, PATH_MAX) << BASHSYMBOL;
            getline(std::cin, *input);
            execLogic(executor);
        } while (exit == false);
        system("clear");
        std::exit(0);
    }
    else
    {
        std::ifstream batchFile(argv[1]); //Instream from file
        std::cout << "Running bash file.\n";
        while (getline(batchFile, *input)) //Get next line as input
        {                                  //until EOF.
            execLogic(executor);
        }
        std::exit(0);
    }
}

/*
 * Used to pull out some of the execution logic to make it a bit
 * easier to read. This was just a bit of duplicate code that I
 * cut out. It handles input until it reaches the handleExec()
 * method.
 * 
 * Parameters: 
 *      executor: Is a refrence to a executor object, so that
 *                it may execute once input is processed.
 * Returns: Nothing
 */
void execLogic(Executor *executor)
{
    tokens = tokenizeInputToCommands(*input); //Parse for semicolons
    for (int i = 0; i < tokens->size(); i++)
    {
        executor->handleExec((*tokens)[i]);
        std::cout << "\n";
    }
}