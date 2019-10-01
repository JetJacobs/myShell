#include "cmdParser.hpp"
#include <iostream>

std::vector<std::string> *tokenizeInput(std::string input)
{
    std::vector<std::string> *command = new std::vector<std::string>();
    char *temp = strtok((char *)input.c_str(), ";");
    while (temp != NULL)
    {
        command->push_back(temp);
        temp = strtok(NULL, ";");
    }
    return command;
}