#include "auto_complete.h"

bool AutoComplete::handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor) {
    if (KeyMap::TerminalAction::ACTION_AUTOCOMPLETE != action)
        return false;
    
    std::vector<std::string> matches = mInterpreter.find(editor.buf());
    if (matches.size() == 0)
        return false;
    editor.set(matches[0]);
    return true;

}
