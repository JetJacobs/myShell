#include "include/cmdParser.hpp"
#include <iostream>

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
