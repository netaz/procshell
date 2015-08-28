/*
 * This file contains the definition of KeyMap
 *
 * For license information, please refer to procshell.h
 */

#ifndef __KEYMAP_H
#define __KEYMAP_H

class KeyMap {
public:
    enum TerminalAction {
        ACTION_INSERT,  // Insert a character 
        ACTION_RIGHT,   // Move the cursor right
        ACTION_LEFT,    // Move the cursor left
        ACTION_HOME,
        ACTION_MODIFIER,
        ACTION_ERASE,   // Delete the previous chracter 
        ACTION_DELETE,  // Delete the current character
        ACTION_UP,
        ACTION_DOWN,
        ACTION_AUTOCOMPLETE,
        ACTION_ABORT,   // Abort line editing
        ACTION_DONE,
        ACTION_IGNORE,
        ACTION_ERROR,
    };

    virtual TerminalAction getAction(unsigned char c, bool escape) = 0;
};

class DosKeyMap : public KeyMap {
public:
    TerminalAction getAction(unsigned char c, bool escape);
};

class TelnetKeyMap : public KeyMap {
public:
    TerminalAction getAction(unsigned char c, bool escape);
};

const unsigned char VT100_ESC = 27;
const unsigned char VT100_CSI[2] = { VT100_ESC, '[' };
class TelnetTerminal;

class Vt100KeyMap : public KeyMap {
    TelnetTerminal &mTerm;
 
public:
    Vt100KeyMap(TelnetTerminal &term) : mTerm(term) {}
    TerminalAction getAction(unsigned char c, bool escape);
};

#endif // __KEYMAP_H
