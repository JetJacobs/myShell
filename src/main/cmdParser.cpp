/* Author Jet Jacobs
 * Date:  10/5/19
 * This file holds all the implementations for parsing all input.
 * Three main parts parsing on whitespace, ;, and |.
 */

#include "include/cmdParser.hpp"
#include <iostream>

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
