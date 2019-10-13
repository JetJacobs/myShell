# MyShell

Jet Jacobs Project 1
Assignment for CS-352

## Running myShell

    The prefered method of running is the following
        1) make
        2) make run
    Additionally you may run with
        1) make
        2) ./myshell
    Running with a batchfile
        1) make
        2) ./myshell "inputname"
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

    1. cd isn't able to be used with pipe or redirection of inputs

    I do know there are more, but these are the ones that stuck out, and may help grading go smoother.

## Retrospective

    So, I think this was a reall fun project. I do tend to prefer to work in higher level languages,
    so that caused some difficulty, but I did learn a lot about how the processes are actually handled.
    At the begining I had a fundemental lack of understanding about both the theory, and implementation
    of pipes. I think this has cleared most (I'm by no means an expert) of the issues I was having.

    Additionally, It isn't as modular as I would have hoped. That being said, it works for the most part,
    but it will need some serious TLC if I want it to be up to par.

    Sugestions
        - More conscise methods
        - Slightly less comments
        - Find a better control flow rather than 100 if statments
