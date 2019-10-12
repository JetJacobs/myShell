/* Author Jet Jacobs
 * Date:  10/5/19
 * This file holds all the declarations for parsing all input.
 * Three main parts parsing on whitespace, ;, and |.
 */

#include <vector>
#include <string>
#include <cstring>

#ifndef CMDPARSER
#define CMDPARSER

/*
 * This is parses the input string on whitespace so that it may
 * be used later during processing. 
 * 
 * Parameters: 
 *      input: Is a simple string expected to be parsed by
 *             ; and | already.
 * Returns: A vector(or "array") of strings, consisting of 
 *          arguments for conversion to be ran in execute()
 */
std::vector<std::string> *parseCommands(std::string input);

/*
 * This is parses the input string on ; so that it may
 * be used later during processing. 
 * 
 * Parameters: 
 *      input: Is a simple string that represents a potential
 *             list of commands.
 * Returns: A vector(or "array") of strings, consisting of 
 *          unique commands, at each index.
 */
std::vector<std::string> *tokenizeInputToCommands(std::string input);

/*
 * This is parses the input string on | so that it may
 * be used later during processing. 
 * 
 * Parameters: 
 *      input: Is a simple string that represents a potential
 *             list of commands to be piped into eachother.
 * Returns: A vector(or "array") of strings, consisting of 
 *          unique commands, that should be piped in order
 *          starting at index 0.
 */
std::vector<std::string> *tokenizePipeCommands(std::string pipeCommand);

#endif