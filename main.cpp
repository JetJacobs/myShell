#include <iostream>
#include <string>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

const std::string BASHSYMBOL = "> ";
const int PATH_MAX = 256;

std::string *currentDir = new std::string();
std::string *command = new std::string();

std::vector<std::string> *tokens;
std::vector<int[2]> pipes;

std::string getcwd();

int main()
{
    bool exit = new bool;
    exit = false;
    *currentDir = getcwd();
    std::vector<std::string> *tokens = new std::vector<std::string>();

    system("clear");
    do
    {
        std::cout << BASHSYMBOL;
        std::cin >> *command;

        if (*command == "quit")
            exit = true;
        else if (*command == "cwd")
            std::cout << *currentDir << "\n";
        else if (*command == "pipetest")
        {
            int childPID = fork();
            if (childPID == 0)
            {
                char *args[2];
                args[0] = (char *)std::string("ls").c_str();
                args[0] = (char *)std::string("&").c_str();
                execvp(args[0], args);
                //execlp("clear", "clear", NULL);
                //execlp("/bin/ls", "ls", NULL);
                return 0;
            }
        }
        else
            std::cout << "Unrecognized command \n";

        wait(NULL);
        wait(NULL);
    } while (exit == false);
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