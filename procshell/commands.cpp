#include "commands.h"
#include "editor.h"
#include "history.h"
#include <sstream>
#include <algorithm>

bool CommandInterpreter::handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor) {
    if (KeyMap::TerminalAction::ACTION_DONE != action)
        return false;

    std::vector<std::string> args = parseCmdLine(editor);
    if (args.size() == 0)
        return false;

    // find cmd matching first arg (command name).
    // remove first arg and pass to cmd.
    std::map<const char*, ShellCommand&>::iterator it = mCmds.begin();
    for (; it != mCmds.end(); it++) {
        if (0 == strcmp(it->first, args[0].c_str())) {
            it->second.handleCommand(args);
            return true;
        }

    }
    
    return false;
}

void CommandInterpreter::registerCommand(ShellCommand &cmd) {
    mCmds.insert( std::pair<const char*, ShellCommand&>(cmd.name(), cmd) );
}

std::vector<std::string> CommandInterpreter::parseCmdLine(LineEditor &editor) {
    std::vector<std::string> args;
    if (editor.buf().length() == 0)
        return args;

    // parse the input line: space serves as a separator, but substrings in quatation mark are
    // treated as whole substrings (not to be chopped up)
    std::string arg;
    std::string cmdline = editor.buf();
    size_t pos = 0;
    std::string delim = " \"";
    while (1) {
        size_t pos2 = cmdline.find_first_of(delim, pos);
        if (pos2 - pos) {
            arg = cmdline.substr(pos, pos2 - pos);
            if (arg.size())
                args.push_back(arg);
        }
        if (pos2 == std::string::npos)
            break;
        if (cmdline[pos2] == '\"' && delim.compare("\"") != 0)  {
            delim = "\"";
        }
        else {
            delim = " \"";
        }
        pos = pos2 + 1;
    }

    return args;
}

std::vector<std::string> CommandInterpreter::findCmd(const std::string &match) const {
    std::vector<std::string> ret;
    if (match.size() == 0)
        return ret;

    std::map<const char*, ShellCommand&>::const_iterator it = mCmds.begin();
    for (; it != mCmds.end(); it++) {
        if (0 == strncmp(match.c_str(), it->first, match.size()))
            ret.push_back(it->first);
    }

    return ret;
}

static size_t edit_distance(const char* a, const char *b) {
    if (strlen(a) == 0)
        return strlen(b);
    if (strlen(b) == 0)
        return strlen(a);

    if (0 == strcmp(a,b))
        return 0;

    size_t cost = (a[0] == b[0]) ? 0 : 1;

    return std::min(cost + edit_distance(a+1, b+1),
                    std::min(1 + edit_distance(a, b+1), 
                             1 + edit_distance(a+1, b) ));
}

std::vector<std::string> CommandInterpreter::approximateFindCmd(const std::string &match) const {
    std::vector<std::string> ret;
    if (match.size() == 0)
        return ret;

    size_t max_dist = std::min((int)match.size(), 1);

    std::map<const char*, ShellCommand&>::const_iterator it = mCmds.begin();
    for (; it != mCmds.end(); it++) {
        if (edit_distance(match.c_str(), it->first) <= max_dist)
            ret.push_back(it->first);
    }

    return ret;
}