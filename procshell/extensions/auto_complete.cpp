#include "auto_complete.h"

bool AutoComplete::handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor) {
    if (KeyMap::TerminalAction::ACTION_AUTOCOMPLETE != action)
        return false;
    
    std::vector<std::string> matches;
    if (mExactMatch)
        matches = mInterpreter.find(editor.buf());
    else
        matches = mInterpreter.approximate_find(editor.buf());
    if (matches.size() == 0)
        return false;
    mTerm.clearInput(editor);
    editor.set(matches[0]);
    return true;

}
