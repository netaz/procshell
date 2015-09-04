#ifndef __CMD_HISTORY_H
#define __CMD_HISTORY_H

#include <vector>
#include "../terminal.h"
#include "../history.h"
#include "../commands.h"

/*
* This sample command provides access to the History from the command-line
*/

class HistoryCmd : public ShellCommand {
    Terminal &mTerm;
    History &mHistory;
public:
    HistoryCmd(Terminal &term, History &history) : mTerm(term), mHistory(history) {}
    const char* name() const { return "history"; }
    CommandStatus handleCommand(const std::vector<std::string> &args);
};

#endif // __CMD_HISTORY_H