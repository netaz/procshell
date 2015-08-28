#include "auto_complete.h"

bool AutoComplete::handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor) {
    if (KeyMap::TerminalAction::ACTION_AUTOCOMPLETE != action)
        return false;
    editor.set("auto complete");
    return true;

}
