#ifndef __HISTORY_H
#define __HISTORY_H

#include "terminal.h"
#include <deque>
#include <string>

/*
 * Command history implementation
 * See: https://en.wikipedia.org/wiki/Command_history
 *
 * Limitations:
 * - Fixed size history
 * - Inserts duplicate commands
 * - No support for serialization/deserialization
 */

class History : public TerminalListener {
    std::deque<std::string> mList;
    int mIter;
    size_t mMaxSize;

public:
    History(size_t maxSize);
    bool handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor);
    void insert(const std::string &newLine);
    bool getCmdString(size_t idx, std::string &str);
};


#endif // __HISTORY