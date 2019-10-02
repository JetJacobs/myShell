#include <iostream>
#include <string>
#include <vector>

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cmdParser.hpp"
#include "executor.hpp"

const std::string BASHSYMBOL = "> ";
const int PATH_MAX = 256;

std::string *currentDir = new std::string();
std::string *command = new std::string();

std::vector<std::string> *tokens;
std::vector<int[2]> pipes;

std::string getcwd();

int main(int argc, char **argv)
{
    bool exit = new bool;
    exit = false;
    *currentDir = getcwd();
    std::vector<std::string> *tokens = new std::vector<std::string>();

    Executor *executor = new Executor();

    system("clear");
    if (argc == 1)
    {
        do
        {
            std::cout << BASHSYMBOL;
            std::cin >> *command;

            if (*command == "quit")
                exit = true;
            else
            {
                tokens = tokenizeInput(*command);
                executor->exeForground(*tokens);
            }
        } while (exit == false);
    }
    system("clear");
    std::exit(0);
}

std::string getcwd()
{
    char buffer[PATH_MAX];
    if (getcwd(buffer, PATH_MAX) != 0)
        return std::string(buffer);
    else
        return "Current working directory not found\n";
}