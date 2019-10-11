#include <iostream>
#include <string>
#include <vector>

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "include/cmdParser.hpp"
#include "include/executor.hpp"

const std::string BASHSYMBOL = "/myshell$ ";
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
            std::cout << "\n"
                      << BASHSYMBOL;
            getline(std::cin, *input);

            // TODO move custom commands to the executor

            if (*input == "quit")
                exit = true;
            else if (*input == "pause")
            {
                getline(std::cin, *input);
            }
            else if (input->rfind("cd ", 0) == 0)
            {
                char *dir = (char *)input->substr(3).c_str();
                if (chdir(dir) == -1)
                    std::cout << "Error Finding the directory.";
            }
            else
            {
                tokens = tokenizeInputToCommands(*input);
                for (int i = 0; i < tokens->size(); i++)
                {
                    executor->handleExec((*tokens)[i]);
                    std::cout << "\n";
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