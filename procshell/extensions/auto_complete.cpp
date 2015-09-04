#include "auto_complete.h"
#include "../editor.h"

bool AutoComplete::handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor) {
    if (KeyMap::TerminalAction::ACTION_AUTOCOMPLETE != action)
        return false;
    
    std::vector<std::string> matches;
    if (mExactMatch)
        matches = mInterpreter.findCmd(editor.buf());
    else
        matches = mInterpreter.approximateFindCmd(editor.buf());
    if (matches.size() == 0)
        return false;
    mTerm.clearInput(editor);
    editor.set(matches[0]);
    return true;

}
