/* Author Jet Jacobs
 * Date:  10/5/19
 * This file holds all the implementations for parsing all input.
 * Three main parts parsing on whitespace, ;, and |.
 */

#include "include/cmdParser.hpp"
#include <iostream>

std::vector<std::string> customCommands = {"clear", "printenv", "ls"};

/*
 * This is parses the input string on whitespace so that it may
 * be used later during processing. 
 * 
 * Parameters: 
 *      input: Is a simple string expected to be parsed by
 *             ; and | already but not required.
 * Returns: A vector(or "array") of strings, consisting of 
 *          arguments for conversion to be ran in execute()
 */
std::vector<std::string> *parseCommands(std::string input)
{
    std::vector<std::string> *commands = new std::vector<std::string>();
    char *temp = strtok((char *)input.c_str(), " \t");
    while (temp != NULL)
    {
        commands->push_back(temp);
        temp = strtok(NULL, " \t");
    }
    return commands;
}

/*
 * This is parses the input string on ; so that it may
 * be used later during processing. 
 * 
 * Parameters: 
 *      input: Is a simple string that represents a potential
 *             list of commands.
 * Returns: A vector(or "array") of strings, consisting of 
 *          unique commands, at each index.
 */
std::vector<std::string> *tokenizeInputToCommands(std::string input)
{
    std::vector<std::string> *commands = new std::vector<std::string>();
    char *temp = strtok((char *)input.c_str(), ";");
    while (temp != NULL)
    {
        commands->push_back(temp);
        temp = strtok(NULL, ";");
    }
    return commands;
}

/*
 * This is parses the input string on | so that it may
 * be used later during processing. 
 * 
 * Parameters: 
 *      input: Is a simple string that represents a potential
 *             list of commands to be piped into eachother.
 * Returns: A vector(or "array") of strings, consisting of 
 *          unique commands, that should be piped in order
 *          starting at index 0.
 */
std::vector<std::string> *tokenizePipeCommands(std::string pipeCommand)
{
    std::vector<std::string> *commands = new std::vector<std::string>();
    char *temp = strtok((char *)pipeCommand.c_str(), "|");
    while (temp != NULL)
    {
        commands->push_back(temp);
        temp = strtok(NULL, "|");
    }
    return commands;
}

/*
 * This formats a command vector into a form that may be passed
 * to a execvp() call.
 * 
 * Parameters: 
 *      command: vector<string> representing arguments to be passed
 * Returns: A formatted vector<char *> that is the passed value
 *          formatted so that it may be passed to an execvp().
 */
std::vector<char *> formatCommandArgs(std::vector<std::string> command)
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
    return argv;
}