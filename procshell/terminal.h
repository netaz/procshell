/*
 * This file contains the definition of Terminal
 *
 * For license information, please refer to procshell.h
 */

#ifndef __TERMINAL__H
#define __TERMINAL__H

#include <iostream>
#include "keymap.h"
class LineEditor;

struct TerminalListener {
    virtual bool handleTerminalAction(KeyMap::TerminalAction action, LineEditor &editor) = 0;
};

class Terminal {
protected:
    std::string mPrompt;
    virtual void displayPrompt();
    // move the printer to its correct position
    virtual void repositionPrinter(LineEditor &editor);
    virtual void printEditLine(LineEditor &editor, bool erase_char = false);
    virtual KeyMap::TerminalAction handleEditAction(KeyMap::TerminalAction action, LineEditor &editor,
                                               bool &escape, unsigned char c);
public:
    Terminal(const std::string prompt);
    virtual KeyMap::TerminalAction collectInput(LineEditor &editor) = 0;
    virtual int putch(char c) = 0;
    virtual unsigned char getch() = 0;
    virtual int puts(const std::string str, bool nl = false) = 0;
    virtual void clearInput(LineEditor &editor);
    virtual void LF();  // Line feed: moves the printer to the next line, keeping the same horizontal position
    virtual void CR();  // Carraige return: moves the printer to the left margin
    virtual void BS();  // Backspace: move the printer one position to the left
};


class LocalTerminal : public Terminal {
    FILE *mInDev;
    FILE *mOutDev;
    DosKeyMap mKeyMap;
    TerminalListener *mListener;
    
public:
    LocalTerminal(const std::string &prompt, TerminalListener *listener);
    KeyMap::TerminalAction collectInput(LineEditor &editor);
    int putch(char c);
    unsigned char getch();
    int puts(const std::string str, bool nl = false);
};

class TelnetTerminal : public Terminal {
    int mSock;
    std::string mPrompt;
    TelnetKeyMap mKeyMap;
    Vt100KeyMap mVt100KeyMap;

    void initiateNegotiation();
    void handleTelnetCommand();
    unsigned char readTelnetOption();
    void NL() { CR(); LF(); }  // new line

public:
    TelnetTerminal(int sock, const std::string &prompt, TerminalListener *listener);
    ~TelnetTerminal();
    KeyMap::TerminalAction TelnetTerminal::collectInput(LineEditor &editor);
    int putch(char c);
    unsigned char getch();
    int puts(const std::string str, bool nl = false);
};


#endif // __TERMINAL__H