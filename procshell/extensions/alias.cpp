#include "alias.h"
#include "../editor.h"

Alias::Alias(CommandInterpreter &interpreter, Terminal &term) :
    mInterpreter(interpreter),
    mTerm(term) {
    interpreter.registerCommand(*this);
}


bool Alias::handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor) {
    auto args = mInterpreter.parseCmdLine(editor);
    auto replace = mAliasDB.find(args[0]);
    if (replace != mAliasDB.end()) {
        std::string &cmdline = editor.buf();
        size_t i = cmdline.find(args[0]);
        cmdline.replace(i, replace->second.length(), replace->second);
        return true;
    }
    return false;
}

void Alias::handleCommand(const std::vector<std::string> &args) {
    // args[0] is the command name
    // args[1] is the aliased cmd
    // args[2] is the new 
    if (args.size() != 3)
        return;
    std::pair<std::string, std::string> alias(args[1], args[2]);
    mAliasDB.insert(alias);
}