#include "keymap.h"
#include <assert.h>

KeyMap::TerminalAction DosKeyMap::getAction(unsigned char c, bool escape) {
    if (!escape) {
        switch (c) {
        case '\b':
            return ACTION_ERASE;
        case '\r':
        case '\n':
            return ACTION_DONE;
        case '\t':
            return ACTION_AUTOCOMPLETE;
        case 3:
            return ACTION_ABORT;
        case 0xe0:
            // When reading a function key or an arrow key, 
            // each function must be called twice; the first call returns 0 or 0xE0, and the second call returns the actual key code. 
            return ACTION_MODIFIER;
        default:
            return ACTION_INSERT;
        }
    }

    assert(escape == true);
    switch (c) {
    case 'K':
        return ACTION_LEFT;
    case 'M':
        return ACTION_RIGHT;
    case 'S':
        return ACTION_DELETE;
    case 'H':
        return ACTION_UP;
    case 'P':
        return ACTION_DOWN;

    default:
        return ACTION_ERROR;
    }
}

KeyMap::TerminalAction TelnetKeyMap::getAction(unsigned char c, bool escape) {
    switch (c) {
        case 127:
            return ACTION_ERASE;

        case '\b':
            return ACTION_ERASE;
        case '\r':
            return ACTION_IGNORE;
        case '\n':
            return ACTION_DONE;
        case '\t':
            return ACTION_AUTOCOMPLETE;
        case 3:
            return ACTION_ABORT;
        case 0xe0:
            // When reading a function key or an arrow key, 
            // each function must be called twice; the first call returns 0 or 0xE0, and the second call returns the actual key code. 
            return ACTION_MODIFIER;
        default:
            return ACTION_INSERT;
    }
}


#include "terminal.h"
KeyMap::TerminalAction Vt100KeyMap::getAction(unsigned char c, bool escape) {
    /// Escape code
    /*
    A control sequence starts with the two-character CSI (Control Sequence Introducer) escape sequence ‘Esc [’,
    followed by an optional sequence of parameter bytes in the range x30–x3F, an optional sequence of intermediate
    bytes in the range x20–x2F (the same range as intermediate bytes in a regular escape sequence), and a final byte in the range x40–x7e.
    The set of standard control sequence functions are defined in Ecma-48 / ISO/IEC 6429.
    */

    // VT-100
    // http://www.databeast.com/datacomet/Documents/1.1._VT100_Command_Set.txt

    c = mTerm.getch();
    
    if (VT100_CSI[1] == c)
    {
        // minimal (not robust) VT100 decoding
        c = mTerm.getch();
        switch (c)
        {
        case '1':
            c = mTerm.getch();
            if ('~' != c)
                break;
            return ACTION_HOME;
        case '3':
            c = mTerm.getch();
            if ('~' != c)
                break;
            return ACTION_DELETE;

        case 'A':
            return ACTION_UP;
        case 'B':
            return ACTION_DOWN;
        case 'C':
            return ACTION_RIGHT;
        case 'D':
            return ACTION_LEFT;
        default:
            break;
        }
    }
    return ACTION_IGNORE;
}
