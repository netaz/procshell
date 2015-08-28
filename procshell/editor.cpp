#include "editor.h"
#include <assert.h>

LineEditor::LineEditor(size_t maxLine) : mMaxInput(maxLine), mCursor(0) {
    assert(maxLine > 0);
}

bool LineEditor::insert(char c) {
    if (mCursor >= mMaxInput)
        return false;
    mLineBuf.insert(mCursor++, &c, 1);
    return true;
}

bool LineEditor::left() {
    if (mCursor > 0) {
        mCursor--;
        return true;
    }
    return false;
}

bool LineEditor::right() {
    if (mCursor < mLineBuf.length()) {
        mCursor++;
        return true;
    }
    return false;
}

bool LineEditor::back() {
    if (mCursor > 0) {
        mCursor--;
        mLineBuf.erase(mLineBuf.begin() + mCursor);
        return true;
    }
    return false;
}

bool LineEditor::del() {
    if (mCursor >= 0 && mCursor<mLineBuf.length()) {
        mLineBuf.erase(mLineBuf.begin() + mCursor);
        return true;
    }

    return false;
}

void LineEditor::clear() {
    mCursor = 0;
    mLineBuf = "";
}

void LineEditor::set(const std::string &newBuf) {
    size_t len = newBuf.length();
    if (len > mMaxInput)
        return;
    mCursor = len;
    mLineBuf = newBuf;
}
