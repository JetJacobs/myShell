void Executor::handlePipes(std::vector<std::string> command)
{
    //Pipe is made
    int fd[2];
    pipe(fd);

    for (int i = 0; i < command.size() - 1; i++)
    {
        pipeStruct *tempPipe = new pipeStruct();
        pipe(tempPipe->ends);
        pipes.push_back(*tempPipe);
        std::cout << command[i] << "\n";
    }

    pid_t childPID;
    int numCmds = command.size();

    for (int i = 0; i < numCmds; i++)
    {
        childPID = fork();
        std::vector<std::string> *arguments = parseCommands(command[i]);

        if (childPID == 0)
        {
            if (i == 0) //if the first command
            {
                dup2(fd[WRITE_END], STDOUT_FILENO);
            }
            else
            {
                dup2(fd[READ_END], STDIN_FILENO);
            }

            //always close both read and write
            close(fd[READ_END]);
            close(fd[WRITE_END]);

            execute(*arguments);
            std::cout << "Error Executing";
            exit(0);
        }
        else if (childPID < 0)
        {
            std::cout << "Error forking";
            exit(1);
        }
        else
        {
            waitpid(childPID, NULL, 0);
        }
    }
}

int Executor::startProcess(int readEnd, int writeEnd, std::string command)
{
    pid_t childPID;
    childPID = fork();

    if (childPID == 0)
    {
        if (readEnd != 0) //if not the first command
        {
            dup2(writeEnd, STDOUT_FILENO);
        }

        if (writeEnd != 1) // if not the last command
        {
            dup2(readEnd, STDIN_FILENO);
        }

        //always close both read and write
        close(writeEnd);
        close(readEnd);

        handleExec(command);
        std::cout << "Error Executing";
        exit(1);
    }
    else if (childPID < 0)
    {
        std::cout << "Error forking";
        exit(1);
    }
    else
    {
        waitpid(childPID, NULL, 0);
    }
}