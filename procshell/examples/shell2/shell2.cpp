/*
* This file contains a sample shell
* This sample attempts to show a more advanced shell, which is packaged as a class.
*
* For license information, please refer to procshell.h
*/
#include <string>
#include <algorithm>
#include <vector>
#include "procshell.h"
#include "extensions/auto_complete.h"
#include "extensions/alias.h"
#include "commands/cmd_history.h"
#include "commands/cmd_exit.h"

class Shell2 {
    Terminal &mTerm;
    LineEditor mEditor;
    History mHistory;
    CommandInterpreter mInterpreter;
    AutoComplete *mAutoComplete;
    Alias *mAlias;
    bool mExit;
    
    void registerCmds();
public:
    Shell2(Terminal &term) : mTerm(term), mEditor(50), mHistory(10), mInterpreter(term), mExit(false) {
        mAutoComplete = new AutoComplete(mInterpreter, term);
        mAlias = new Alias(mInterpreter, term);
    }

    void run();
};

void Shell2::registerCmds() {
    ShellCommand *cmds[] = {
        new HistoryCmd(mTerm, mHistory),
        new ExitCmd(mTerm, mExit)
    };

    for (auto cmd : cmds) {
        mInterpreter.registerCommand(*cmd);
    }
}

void Shell2::run() {
    registerCmds();
    KeyMap::TerminalAction action;

    while (!mExit) {
        action = mTerm.collectInput(mEditor);

        switch (action) {
        case KeyMap::TerminalAction::ACTION_UP:
        case KeyMap::TerminalAction::ACTION_DOWN:
            mTerm.clearInput(mEditor);
            mHistory.handleTerminalAction(action, mEditor);
            break;
        case KeyMap::TerminalAction::ACTION_AUTOCOMPLETE:
            mAutoComplete->handleTerminalAction(action, mEditor);
            break;

        case KeyMap::TerminalAction::ACTION_ABORT:
            mTerm.LF(); mTerm.CR();
            mEditor.clear();
            break;

        case KeyMap::TerminalAction::ACTION_DONE:
            mTerm.LF(); mTerm.CR();
            mHistory.handleTerminalAction(action, mEditor);
            mAlias->handleTerminalAction(action, mEditor);
            mInterpreter.handleTerminalAction(action, mEditor);
            mEditor.clear();
            break;
        }
    }
}

int main(int argc, const char* argv[])
{
    LocalTerminal term("shell2> ", 0);
    Shell2 shell(term);
    shell.run();
    return EXIT_SUCCESS;
}