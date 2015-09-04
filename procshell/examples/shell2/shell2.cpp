/*
* This file contains a sample shell
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

#ifdef _WIN32
//#include "stdafx.h"
#endif

/*
* This sample shows how a shell with some basic functionality is assembled
*
*/

int shell(Terminal &term) {
    LineEditor editor(50);
    KeyMap::TerminalAction action;
    History history(10);
    CommandInterpreter interpreter(term);
    AutoComplete complete(interpreter, term);
    HistoryCmd historyCmd(term, history);
    bool exit;
    ExitCmd exitCmd(term, exit);
    Alias alias(interpreter, term);
    interpreter.registerCommand(historyCmd);
    interpreter.registerCommand(exitCmd);

    while (!exit) {
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
            alias.handleTerminalAction(action, editor);
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