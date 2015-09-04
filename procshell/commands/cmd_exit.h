#ifndef __EXIT_CMD_H
#define __EXIT_CMD_H

#include "../terminal.h"
#include "../commands.h"

class ExitCmd : public ShellCommand {
    Terminal &mTerm;
    bool &mExitFlag;
public:
    ExitCmd(Terminal &term, bool &exit) : mTerm(term), mExitFlag(exit) { exit = false; }
    const char* name() const { return "exit"; }
    CommandStatus handleCommand(const std::vector<std::string> &args) {
        mExitFlag = true;
        return ShellCommand::CommandSuccess;
    }
};

#endif // __EXIT_CMD_H