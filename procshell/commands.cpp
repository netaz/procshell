#include "commands.h"
#include "editor.h"
#include "history.h"
#include <sstream>

bool CommandInterpreter::handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor) {
    if (KeyMap::TerminalAction::ACTION_DONE != action)
        return false;

    if (editor.buf().length() == 0)
        return false;

    // parse the input line using SPACE separator
    std::istringstream is(editor.buf());
    std::string arg;
    std::vector<std::string> args;
    while (getline(is, arg, ' ')) {
        if (arg.size() > 0) args.push_back(arg);
    }

    // find cmd matching first arg (command name)
    // remove first arg and pass to cmd
    std::map<const char*, ShellCommand&>::iterator it = mCmds.begin();
    for (; it != mCmds.end(); it++) {
        if (0 == strcmp(it->first, args[0].c_str())) {
            it->second.handleCommand(args);
            return true;
        }

    }
    
    return false;
}

void CommandInterpreter::registerCommnad(ShellCommand &cmd) {
    mCmds.insert( std::pair<const char*, ShellCommand&>(cmd.name(), cmd) );
}