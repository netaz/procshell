/*
* This file contains the definition of Alias, a sample shell extension.
* Alias ('alias') is a somewhat special extension because it looks like a
* command.  Indeed, part of its functionality implements a special breed of
* commands, referred to as built-in commands.
*
* For license information, please refer to procshell.h
*/
#ifndef __ALIAS_H
#define __ALIAS_H

#include "../terminal.h"
#include "../commands.h"
#include <map>

class Alias : public TerminalListener, ShellCommand {
    CommandInterpreter &mInterpreter;
    Terminal &mTerm;
    std::map<std::string, std::string> mAliasDB;
public:
    Alias(CommandInterpreter &interpreter, Terminal &term);
    bool handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor);
    const char* name() const { return "alias"; }
    void handleCommand(const std::vector<std::string> &args);
};

#endif // __ALIAS_H