/*
 * This file contains the definition of LineEditor
 *
 * For license information, please refer to procshell.h
 */

#ifndef __EDITOR_H
#define __EDITOR_H

#include <string>

class LineEditor {
public:
    LineEditor(size_t maxLine);
    bool insert(char c);
    bool left();
    bool right();
    bool back();
    bool del();
    void clear();
    void set(const std::string &newBuf);

    const std::string& buf() const { return mLineBuf; }
    size_t cursor() const { return mCursor; }

private:
    std::string mLineBuf;
    size_t mMaxInput;
    size_t mCursor;

    LineEditor(const LineEditor&);
public:
};


#endif //__EDITOR_H