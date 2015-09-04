/*
 * This file contains the definition of AutoComplete, a sample shell extension.
 * This shows how the shell functionality can be extended.
 *
 * For license information, please refer to procshell.h
 */
#ifndef __AUTO_COMPLETE_H
#define __AUTO_COMPLETE_H

#include "../terminal.h"
#include "../commands.h"

/*
 * Command name completion
 * See: https://en.wikipedia.org/wiki/Comparison_of_command_shells#Completions
 */
class AutoComplete : public TerminalListener {
    CommandInterpreter &mInterpreter;
    Terminal &mTerm;
    bool mExactMatch;
public:
    AutoComplete(CommandInterpreter &interpreter, Terminal &term, bool exactMatch=true) : 
        mInterpreter(interpreter), 
        mTerm(term),
        mExactMatch(exactMatch) {}
    bool handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor);
};

#endif // __AUTO_COMPLETE_H
