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
    void executeInitScript();

public:
    Shell2(Terminal &term) : mTerm(term), mEditor(50), mHistory(10), mInterpreter(term), mExit(false) {
        mAutoComplete = new AutoComplete(mInterpreter, term);
        mAlias = new Alias(mInterpreter, term);
    }

    void run();
    ShellCommand::CommandStatus handleSetCommand(const std::vector<std::string> &args);

    // 'set' built-in command, which alters the internal configuration of the shell itself.
    class SetCmd : public ShellCommand {
        Shell2 &mShell;
    public:
        SetCmd(Shell2 &shell) : mShell(shell) {}
        CommandStatus handleCommand(const std::vector<std::string> &args) { return mShell.handleSetCommand(args); }
        const char* name() const { return "set"; }
    };

};

void Shell2::registerCmds() {
    ShellCommand *cmds[] = {
        new HistoryCmd(mTerm, mHistory),
        new ExitCmd(mTerm, mExit),
        new Shell2::SetCmd(*this),
    };

    for (auto cmd : cmds) {
        mInterpreter.registerCommand(*cmd);
    }
}

// Just like ~/.bashrc, you might one to run a script before starting your shell
void Shell2::executeInitScript() {
    const char *script[] = { "alias quit exit", };

    // Let the CLI interpreter interpret line by line from the script (I'm not checking for errors)
    LineEditor editor(50);
    for (auto line : script) {
        editor.set(line);
        mInterpreter.handleTerminalAction(KeyMap::TerminalAction::ACTION_DONE, editor);
    }
}

void Shell2::run() {
    registerCmds();
    executeInitScript();
    
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
            if (mEditor.buf().size() == 0)
                break;

            // insert command line to the history
            mHistory.handleTerminalAction(action, mEditor);
            // replace aliased commands with their alias value
            mAlias->handleTerminalAction(action, mEditor);
            // let the interpreter do its thing
            mInterpreter.handleTerminalAction(action, mEditor);
            mEditor.clear();
            break;
        }
    }
}

ShellCommand::CommandStatus Shell2::handleSetCommand(const std::vector<std::string> &args) {
    if (args.size()>2 && args[1].compare("prompt") == 0) {
        if (args.size() == 4 && args[2].compare("=")==0) {
            // syntax: 'set prompt = arg[3] '
            mTerm.prompt(args[3]);
            return ShellCommand::CommandSuccess;
        }
        if (args.size() == 3) {
            // syntax: 'set prompt arg[2] '
            mTerm.prompt(args[2]);
            return ShellCommand::CommandSuccess;
        }
    }
    
    return ShellCommand::CommandFailure_BadParams;
}

int main(int argc, const char* argv[])
{
    LocalTerminal term("shell2> ");
    Shell2 shell(term);
    shell.run();
    return EXIT_SUCCESS;
}