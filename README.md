# MyShell

Assignment for CS-352

# Running myShell

    The prefered method of running is the following
        1) make
        2) make run
    Additionally you may run with
        1) make
        2) ./myshell.o

    Running with input and output files
        1) make
        2) ./myshell.o

# Known bugs

    1. When piping into the *more* command:
        If the data piped into the more command is smaller than the terminal, then it hangs.

        I believe this is do to a abnormal termination in the more command, but was unsure how to fix.

    2. Running *help*:
        It appears this is not a supported command via execvp. I was unable to confirm how this works.
        I intend to instead list this page.
