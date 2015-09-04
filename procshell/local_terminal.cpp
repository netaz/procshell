#include "terminal.h"
#include "editor.h"
#include <cstdio>
#include <conio.h>

Terminal::Terminal(const std::string prompt) : mPrompt(prompt) {}

void Terminal::prompt(const std::string prompt) { mPrompt = prompt;  }

void Terminal::LF() {
    putch('\n');
}

void Terminal::CR() {
    putch('\r');
}

void Terminal::BS() {
    putch('\b');
}

void Terminal::clearInput(LineEditor &editor) {
    CR();
    size_t len = editor.buf().length() + mPrompt.length();
    for (size_t i = 0; i < len; i++)
        putch(' ');
}

void Terminal::displayPrompt() {
    puts(mPrompt);
}

void Terminal::repositionPrinter(LineEditor &editor) {
    size_t back = editor.buf().length() - editor.cursor();
    while (back) { BS(); back--; }
}

void Terminal::printEditLine(LineEditor &editor, bool erase_char) {
    CR();
    displayPrompt();
    // print the new edit line
    const char *buf = editor.buf().c_str();
    while (*buf) { putch(*buf++); }

    // erase the extra character if the user shortened the input line
    if (erase_char) {
        putch(' ');
        BS();
    }

    repositionPrinter(editor);
}

KeyMap::TerminalAction Terminal::handleEditAction(KeyMap::TerminalAction action, 
                                                  LineEditor &editor,
                                                  bool &escape, unsigned char c) {
    bool print_line = false;
    bool erase_char = false;

    switch (action) {
    case KeyMap::TerminalAction::ACTION_INSERT:
        if (editor.insert(c))
            print_line = true;
        break;
    case KeyMap::TerminalAction::ACTION_MODIFIER:
        // read the next char
        escape = true;
        break;
    case KeyMap::TerminalAction::ACTION_LEFT:
        if (editor.left())
            print_line = true;
        break;
    case KeyMap::TerminalAction::ACTION_RIGHT:
        if (editor.right())
            print_line = true;
        break;
    case KeyMap::TerminalAction::ACTION_ERASE:
        if (editor.back()) {
            print_line = true;
            erase_char = true;
        }
        break;
    case KeyMap::TerminalAction::ACTION_DELETE:
        if (editor.del()) {
            print_line = true;
            erase_char = true;
        }
        break;
    default:
        return action;
    }

    if (print_line) {
        printEditLine(editor, erase_char);
    }

    return KeyMap::TerminalAction::ACTION_IGNORE;
}

LocalTerminal::LocalTerminal(const std::string &prompt) :
    Terminal(prompt),
    mInDev(stdin),
    mOutDev(stdout) {
}

KeyMap::TerminalAction LocalTerminal::collectInput(LineEditor &editor) {
    unsigned char c = 0;
    KeyMap::TerminalAction action;
    bool escape = false;

    CR();
    displayPrompt();
    printEditLine(editor);
    while (true) {
        action = mKeyMap.getAction(c = getch(), escape);
        escape = false;

        // Here only handle local line editing events.
        // Propagate all other events to higher processing layers
        action = handleEditAction(action, editor, escape, c);
        if (action != KeyMap::TerminalAction::ACTION_IGNORE)
            return action;
    }
}



int LocalTerminal::puts(const std::string str, bool nl) {
    int i = fwrite(str.c_str(), sizeof(char), str.size(), mOutDev);
    if (nl) {
        LF(); CR();
    }
    return i;
}

int LocalTerminal::putch(char c) {
    return _putch(c);
}

unsigned char LocalTerminal::getch() {
    return _getch();
}