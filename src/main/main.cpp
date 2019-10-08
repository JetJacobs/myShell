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

            /* TODO move custom commands to the executor
             * 
             * 
            */
            if (*input == "quit")
                exit = true;
            else
            {
                tokens = tokenizeInputToCommands(*input);
                for (int i = 0; i < tokens->size(); i++)
                {
                    executor->handleExec((*tokens)[i]);
                }
            }
        } while (exit == false);
    }
    else
    {
        std::cout << "Run bash file.";
    }
    system("clear");
    std::exit(0);
}