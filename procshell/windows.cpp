#include <Winsock2.h>

int InitSockets()
{
    static int WinInitialized = 0;
    WSADATA WsaData;
    if (!WinInitialized)
    if (WSAStartup((unsigned short)1.1, &WsaData) != 0)
    {
        return -1;
    }
    else
        WinInitialized = 1;

    return 0;
}
