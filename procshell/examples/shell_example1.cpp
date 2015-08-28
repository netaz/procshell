/*
 * This file contains a sample command (history) and a sample shell
 * 
 * For license information, please refer to procshell.h
 */
#include <string>
#include <algorithm>
#include <vector>
#include "../procshell.h"
#include "../extensions/auto_complete.h"

#ifdef _WIN32
#include "stdafx.h"
#endif

/*
 * This sample command provides access to the History from the command-line
 */

class HistoryCmd : public ShellCommand {
    Terminal &mTerm;
    History &mHistory;
public:
    HistoryCmd(Terminal &term, History &history) : mTerm(term), mHistory(history) {}
    const char* name() const { return "history"; }
    void handleCommand(const std::vector<std::string> &args);
};

void HistoryCmd::handleCommand(const std::vector<std::string> &args) {
    std::string cmd;
    size_t i = 0;
    while (mHistory.getCmdString(i++, cmd))
        mTerm.puts(cmd.c_str(), true);
}


/*
 * This sample shows how a shell with some basic functionality is assembled
 */

int shell(Terminal &term) {
    LineEditor editor(15);
    KeyMap::TerminalAction action;
    History history(10);
    AutoComplete complete;
    CommandInterpreter interpreter(term);
    HistoryCmd historyCmd(term, history);
    interpreter.registerCommnad(historyCmd);

    while (1) {
        action = term.collectInput(editor);

        switch (action) {
        case KeyMap::TerminalAction::ACTION_UP:
        case KeyMap::TerminalAction::ACTION_DOWN:
            term.clearInput(editor);
            history.handleTerminalAction(action, editor);
            break;
        case KeyMap::TerminalAction::ACTION_AUTOCOMPLETE:
            complete.handleTerminalAction(action, editor);
            break;

        case KeyMap::TerminalAction::ACTION_ABORT:
            term.LF(); term.CR();
            editor.clear();
            break;

        case KeyMap::TerminalAction::ACTION_DONE:
            term.LF(); term.CR();
            history.handleTerminalAction(action, editor);
            interpreter.handleTerminalAction(action, editor);
            editor.clear();
            break;
        }
    }
    return 0;
}

int LaunchLocalCLI() {

    LocalTerminal term("machine> ", 0);
    shell(term);
    return EXIT_SUCCESS;
}

int TelnetServer();

int LaunchTelnetCLI() {
    int sock = TelnetServer();
    if (0 == sock)
        return EXIT_FAILURE;

    TelnetTerminal term(sock, "kishkush>", 0);
    shell(term);
    return EXIT_SUCCESS;
}

int main(int argc, const char* argv[])
{
    //    return LaunchTelnetCLI();
    return LaunchLocalCLI();
}

