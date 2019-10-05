#include <vector>
#include <string>
#include <cstring>

#ifndef CMDPARSER
#define CMDPARSER

std::vector<std::string> *tokenizeInputToCommands(std::string input);
std::vector<std::string> *parseCommands(std::string input);

#endif