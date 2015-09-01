/*
 * This file contains the definition of AutoComplete, a sample shell extension.
 * This shows how the shell functionality can be extended.
 *
 * For license information, please refer to procshell.h
 */
#ifndef __AUTO_COMPLETE_H
#define __AUTO_COMPLETE_H

#include "../procshell.h"

/*
 * Command name completion
 * See: https://en.wikipedia.org/wiki/Comparison_of_command_shells#Completions
 */
class AutoComplete : public TerminalListener {
    CommandInterpreter &mInterpreter;
public:
    AutoComplete(CommandInterpreter &interpreter) : mInterpreter(interpreter) {}
    bool handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor);
};

#endif // __AUTO_COMPLETE_H
