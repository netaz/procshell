/*
 * This file contains the definition of ShellCommand and CommandInterpreter.
 * Commands consist of a list of strings or words interpreted as a command name
 * followed by arguments.
 *
 * For license information, please refer to procshell.h
 */

#ifndef __COMMANDS_H
#define __COMMANDS_H

#include "terminal.h"
#include <map>
#include <string>
#include <vector>

class ShellCommand {
public:
    virtual void handleCommand(const std::vector<std::string> &args) = 0;
    virtual const char* name() const = 0;
};

/*
 * Define a string ordering so we can later traverse the commands in order
 */
struct string_cmp {
    bool operator()(const char *a, const char *b) {
        return strcmp(a, b) < 0 ? true : false;
    }
};

class CommandInterpreter : public TerminalListener {
    Terminal &mTerm;
    std::map<const char*, ShellCommand&, string_cmp> mCmds;
public:
    CommandInterpreter(Terminal &term) : mTerm(term) {}
    void registerCommand(ShellCommand &cmd);
    bool handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor);
    // Perform a partial string search and return all matches
    std::vector<std::string> find(const std::string &match) const;
};


#endif // __COMMANDS_H