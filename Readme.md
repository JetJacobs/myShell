# MyShell

Jet Jacobs Project 1
Assignment for CS-352

## Running myShell

    The prefered method of running is the following
        1) make
        2) make run
    Additionally you may run with
        1) make
        2) ./myshell.o
    Running with a batchfile
        1) make
        2) ./myshell.o "inputname"
        Substituting the values in quotes with the expected values

## Testing

    This was a smaller project, and I didn't want to spend the time to use a legitamite testing library.
    As such I have made a few files that run really basic tests on both the parser and executor.
        For a parser test run
            1) make parserTest
        For a executor test run
            1) make execTest
        For both run
            1) make test

## Known bugs

    1. When piping into the more command:
        If the data piped into the more command is smaller than the terminal, then it hangs.
        Due to it working otherwise, I believe this is do to a abnormal termination.
        in the more command for this scenario. I was unsure how to fix.

    2. Running help:
        It appears this is not a supported command via execvp. I was unable to confirm how this works.
        I intend to instead list the read me page.

    I do know there are more, but these are the ones that stuck out, and may help grading go smoother.

## Additional Notes

    I had some trouble discerning what to print as part of my shell.
    I didn't know if the line was to change as we navigate to other directories, or if it was to be the location it was executed from.
