#include "history.h"
#include "keymap.h"
#include "editor.h"
#include <algorithm>

History::History(size_t maxSize) : 
    mIter(-1), 
    mMaxSize(maxSize) {}

void History::insert(const std::string &newLine) {
    if (mList.size() == mMaxSize)
        mList.pop_back();
    mList.push_front(newLine);
}

bool History::handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor) {
    switch (action) {
    case KeyMap::TerminalAction::ACTION_UP:
        if (mList.size() == 0)
            return true;

        mIter = std::min<int>(mIter + 1, mList.size() - 1);
        editor.set(mList[mIter]);
        return true;
    case KeyMap::TerminalAction::ACTION_DOWN:
        if (mList.size() == 0)
            break;
        mIter = std::max(mIter - 1, 0);
        editor.set(mList[mIter]);
        return true;
    case KeyMap::TerminalAction::ACTION_DONE:
        insert(editor.buf());
        mIter = -1;
        return true;
    }

    return false;
}

bool History::getCmdString(size_t idx, std::string &str) {
    if (idx >= mList.size())
        return false;

    str = mList[idx];
    return true;
}
