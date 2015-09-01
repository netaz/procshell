# procshell
ProcShell provides a modular and extensible in-process shell infrastructure.  

In-process shells are used in situations where you'd like to give the 
application user a command-line interface (CLI) to control, configure or expose
the inner state of your process.

From the perspecitve of your application, procsheel is a View/Controller accessing
a Model (your app's business logic) via a CLI.

The terminology within the realm of shells/consoles/terminals is quite confusing
because concepts are often used interchangibly.  This is how I interpret the concepts:
- A terminal provide text input/output interaction with the user.
- A console is a type of physical terminal.
- A shell is a command-line interpreter.
- The terminal is the frontend, while the shell is the backend.

Abstractions (class names are capitalized):
- A Terminal provides means for text input/output.  For example, a LocalTerminal 
uses the keyboard and screen for I/O, while (the incomplete) TelnetTerminal uses
a Telnet connection for I/O with a remote keyboard and screen.  A Terminal uses 
a LineEditor to manage the editing of the current command-line.  
- A Keymap maps key-strokes to a well-known set of TerminalActions.  This mapping is
not restricted to physical keys-strokes, as in the case of mapping VT100 codes to
TerminalActions.  This mapping mechanism provides loose-coupling between the main 
ProcShell abstractions and the terminal implementation.
- History stores a cyclic list of the latest executed commands and provides an interface
to navigate this list.  I've made History part of the procshell core, although it could
have very well been an extension.  Of course, as you assemble your 

For a discussion of an actual Linux shell, see:
http://www.kitebird.com/csh-tcsh-book/csh-intro.pdf

Most Telnet programs implement a shell.  Microsoft's Telnet shell commands are described
here: https://technet.microsoft.com/en-us/library/bb491013.aspx

Cisco routers provide a CLI for managing the device.  Some documentation with examples:
http://www.cisco.com/c/en/us/td/docs/routers/access/1900/software/configuration/guide/Software_Configuration/routconf.html