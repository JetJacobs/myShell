#include <iostream>
#include <string>
#include <vector>

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "include/cmdParser.hpp"
#include "include/executor.hpp"

const std::string BASHSYMBOL = "> ";
const int PATH_MAX = 256;

std::string *input = new std::string();
std::vector<std::string> *tokens;
std::vector<int[2]> pipes;

int main(int argc, char **argv)
{
    bool exit = new bool;
    exit = false;
    tokens = new std::vector<std::string>();

    Executor *executor = new Executor();

    system("clear");
    if (argc == 1)
    {
        do
        {
            std::cout << BASHSYMBOL;
            getline(std::cin, *input);

            if (*input == "quit")
                exit = true;
            else
            {
                tokens = tokenizeInputToCommands(*input);
                for (int i = 0; i < tokens->size(); i++)
                {
                    std::vector<std::string> *command = parseCommands((*tokens)[i]);
                    executor->exeForground(*command);
                }
            }
        } while (exit == false);
    }
    system("clear");
    std::exit(0);
}

/*std::string *currentDir = new std::string();
std::string getcwd();
std::string getcwd()
{
    char buffer[PATH_MAX];
    if (getcwd(buffer, PATH_MAX) != 0)
        return std::string(buffer);
    else
        return "Current working directory not found\n";
}*/