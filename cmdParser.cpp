#include "cmdParser.hpp"
#include <iostream>

std::vector<std::string> *tokenizeInput(std::string input)
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
