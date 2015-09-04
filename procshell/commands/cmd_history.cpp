#include "cmd_history.h"

ShellCommand::CommandStatus HistoryCmd::handleCommand(const std::vector<std::string> &args) {
    std::string cmd;
    size_t i = 0;
    // traverse the history list and print the command strings
    while (mHistory.getCmdString(i++, cmd))
        mTerm.puts(cmd.c_str(), true);
    return CommandSuccess;
}

