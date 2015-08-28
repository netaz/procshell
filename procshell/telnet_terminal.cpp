#include "terminal.h"
#include "editor.h"
#include <Winsock2.h>

int InitSockets();
#define PROCSHELL_PORT 54321

int TelnetServer()
{
    InitSockets();
    struct sockaddr_in servaddr;
    int serverSock, connectionSock;
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    // CLI service port
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PROCSHELL_PORT);
    bind(serverSock, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // wait for a connection
    listen(serverSock, 50);

    if (connectionSock = accept(serverSock, NULL, 0)) {
        return connectionSock;
    }
    return 0;
}

void LOG_RX(const char *cmd, unsigned char option) {
    printf("RX: %s %u\n", cmd, option);
}

void LOG_TX(unsigned char cmd, unsigned char option) {
    printf("TX: %d %d\n", cmd, option);
}

/*
    Telnet protocol
    Caveat: Telnet implementation is not robust (in the sense of its coverage of the protocol)

 */

enum Verbs {
    WILL = 251,
    WONT = 252,
    DO = 253,
    DONT = 254,
    IAC = 255
};

enum Options
{
    ECHO = 1,
    SGA = 3,
};


TelnetTerminal::TelnetTerminal(int sock, const std::string &prompt, TerminalListener *listener) :
    Terminal(prompt), 
    mSock(sock),
    mVt100KeyMap(*this) {
    initiateNegotiation();
}

TelnetTerminal::~TelnetTerminal() {
    closesocket(mSock);
}

// RFC1123: When the Telnet protocol is used in a client/server situation, 
// the server SHOULD initiate negotiation of the terminal interaction mode it expects.
void TelnetTerminal::initiateNegotiation()
{
    unsigned char cmd[3];
    cmd[0] = Verbs::IAC;
    cmd[1] = Verbs::DO;
    cmd[2] = Options::SGA;
    send(mSock, (const char*)cmd, sizeof(cmd), 0);

    cmd[0] = Verbs::IAC;
    cmd[1] = Verbs::DONT;
    cmd[2] = Options::ECHO;
    send(mSock, (const char*)cmd, sizeof(cmd), 0);

    cmd[0] = Verbs::IAC;
    cmd[1] = Verbs::DONT;
    cmd[2] = 10;
    send(mSock, (const char*)cmd, sizeof(cmd), 0);
}


int TelnetTerminal::putch(char c) {
    return send(mSock, &c, sizeof(char), 0);
}

unsigned char TelnetTerminal::getch() {
    unsigned char c;
    recv(mSock, (char*)&c, 1, 0);
    return c;
}

int TelnetTerminal::puts(const std::string str, bool nl) {
    int i = send(mSock, str.c_str(), str.size(), 0);
    if (nl) 
        NL();

    return i;
}

KeyMap::TerminalAction TelnetTerminal::collectInput(LineEditor &editor) {
    unsigned char c = 0;
    KeyMap::TerminalAction action;
    bool escape = false;

    CR();
    displayPrompt();
    printEditLine(editor);
    while (true) {
        if (recv(mSock, (char*)&c, 1, 0) <= 0)
            return KeyMap::TerminalAction::ACTION_ERROR;

        switch (c) {
        case Verbs::IAC:
            handleTelnetCommand();
            break;
        default:
            if (c == VT100_ESC)
                action = mVt100KeyMap.getAction(c, escape);
            else
                action = mKeyMap.getAction(c, escape);

            // Here only handle local line editing events.
            // Propagate all other events to higher processing layers
            action = handleEditAction(action, editor, escape, c);
            if (action != KeyMap::TerminalAction::ACTION_IGNORE)
                return action;
        }
    }
}

unsigned char TelnetTerminal::readTelnetOption()
{
    char inputoption;
    recv(mSock, &inputoption, 1, 0);
    return inputoption;
}

void TelnetTerminal::handleTelnetCommand() {

    // interpret as command
    unsigned char inputverb;
    recv(mSock, (char*)&inputverb, 1, 0);

    //if (inputverb == -1) break;
    switch (inputverb)
    {
        case (int)Verbs::DO:
        {
            unsigned char option = readTelnetOption();
            LOG_RX("DO", option);
            // reply to all commands with "WONT", unless it is SGA (suppres go ahead) or ECHO
            unsigned char cmd[3];
            cmd[0] = Verbs::IAC;
            cmd[1] = Verbs::WONT;
            cmd[2] = option;
            if (Options::SGA == option || Options::ECHO == option)
                cmd[1] = Verbs::WILL;

            send(mSock, (const char*)cmd, sizeof(cmd), 0);
            LOG_TX(cmd[1], cmd[2]);
        }
        break;

        case (int)Verbs::DONT:
            LOG_RX("DONT", readTelnetOption()); break;
        case (int)Verbs::WILL:
            LOG_RX("WILL", readTelnetOption()); break;
        case (int)Verbs::WONT:
            LOG_RX("WONT", readTelnetOption()); break;
        break;

    default:
        LOG_RX("Unknown command\n", 0);
        break;
    }
}
