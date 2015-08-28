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


class CommandInterpreter : public TerminalListener {
    Terminal &mTerm;
    std::map<const char*, ShellCommand&> mCmds;
public:
    CommandInterpreter(Terminal &term) : mTerm(term) {}
    void registerCommnad(ShellCommand &cmd);
    bool handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor);
};


#endif // __COMMANDS_H