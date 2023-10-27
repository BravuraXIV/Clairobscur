#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif // _MSC_VER

#ifdef _WIN32
#include <winsock2.h>
#endif // _WIN32

#include "server.h"
#include "../logging.h"

struct client
{
    int socket;
    bool online = false;
};

internal client clients[100];

internal int make_socket(const char* host, u16 port);
internal void start_accept_thread();
internal void start_recv_thread();
internal void start_packet_processing();

int run_server()
{
    return run_server("127.0.0.1", 54994);
}

int run_server(const char* host, u16 port)
{
#ifdef _WIN32
    WORD wsaVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    WSAStartup(wsaVersion, &wsaData);
#endif
    
    int sock = make_socket(host, port);
    if (sock == -1)
    {
        return 1;
    }
    
    while (true)
    {
        sockaddr addr_in;
        int addr_in_len;
        auto cli_sock = accept(sock, &addr_in, &addr_in_len);
#ifdef _WIN32
        if (cli_sock == INVALID_SOCKET)
        {
            break;
        }
#endif
        bravura_log("New client!");
    }

    WSACleanup();
    return 0;
}

internal int make_socket(const char* host, u16 port)
{
    // Setup Winsock.
    {
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        int err = WSAStartup(wVersionRequested, &wsaData);

        switch (err)
        {
        case 0: break; // No error
        case WSASYSNOTREADY:
            bravura_log_err("The underlying network subsystem is not ready for network communication.");
            return -1;
        case WSAVERNOTSUPPORTED:
            bravura_log_err("The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.");
            return -1;
        case WSAEINPROGRESS:
            bravura_log_err("A blocking Windows Sockets 1.1 operation is in progress.");
            return -1;
        case WSAEPROCLIM:
            bravura_log_err("A limit on the number of tasks supported by the Windows Sockets implementation has been reached.");
            return -1;
        case WSAEFAULT:
            bravura_log_err("The lpWSAData parameter is not a valid pointer.");
            return -1;
        }

        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
            bravura_log_err("Could not find a matching winsock version");
            WSACleanup();
            return -1;
        }
    }

    auto sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
    {
        int err = WSAGetLastError();
        switch (err)
        {
        case WSANOTINITIALISED:
            bravura_log_err("A successful WSAStartup call must occur before using this function.");
            break;
        case WSAENETDOWN:
            bravura_log_err("The network subsystem or the associated service provider has failed.");
            break;
        case WSAEAFNOSUPPORT:
            bravura_log_err("The specified address family is not supported. For example, an application tried to create a socket for the AF_IRDA address family but an infrared adapter and device driver is not installed on the local computer.");
            break;
        case WSAEINPROGRESS:
            bravura_log_err("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
            break;
        case WSAEMFILE:
            bravura_log_err("No more socket descriptors are available.");
            break;
        case WSAEINVAL:
            bravura_log_err("An invalid argument was supplied. This error is returned if the af parameter is set to AF_UNSPEC and the type and protocol parameter are unspecified.");
            break;
        case WSAEINVALIDPROVIDER:
            bravura_log_err("The service provider returned a version other than 2.2.");
            break;
        case WSAEINVALIDPROCTABLE:
            bravura_log_err("The service provider returned an invalid or incomplete procedure table to the WSPStartup.");
            break;
        case WSAENOBUFS:
            bravura_log_err("No buffer space is available. The socket cannot be created.");
            break;
        case WSAEPROTONOSUPPORT:
            bravura_log_err("The specified protocol is not supported.");
            break;
        case WSAEPROTOTYPE:
            bravura_log_err("The specified protocol is the wrong type for this socket.");
            break;
        case WSAEPROVIDERFAILEDINIT:
            bravura_log_err("The service provider failed to initialize. This error is returned if a layered service provider (LSP) or namespace provider was improperly installed or the provider fails to operate correctly.");
            break;
        case WSAESOCKTNOSUPPORT:
            bravura_log_err("The specified socket type is not supported in this address family.");
        }
        return -1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);
    if (bind(sock, (SOCKADDR*) &addr, sizeof(addr)) == SOCKET_ERROR)
    {
        auto err = WSAGetLastError();
        switch (err)
        {
        case WSANOTINITIALISED:
            bravura_log_err("A successful WSAStartup call must occur before using this function. ");
            break;
        case WSAENETDOWN:
            bravura_log_err("The network subsystem has failed.");
            break;
        case WSAEACCES:
            bravura_log_err("An attempt was made to access a socket in a way forbidden by its access permissions. This error is returned if nn attempt to bind a datagram socket to the broadcast address failed because the setsockopt option SO_BROADCAST is not enabled.");
            break;
        case WSAEADDRINUSE:
            bravura_log_err("Only one usage of each socket address (protocol/network address/port) is normally permitted. This error is returned if a process on the computer is already bound to the same fully qualified address and the socket has not been marked to allow address reuse with SO_REUSEADDR. For example, the IP address and port specified in the name parameter are already bound to another socket being used by another application. For more information, see the SO_REUSEADDR socket option in the SOL_SOCKET Socket Options reference, Using SO_REUSEADDR and SO_EXCLUSIVEADDRUSE, and SO_EXCLUSIVEADDRUSE.");
            break;
        case WSAEADDRNOTAVAIL:
            bravura_log_err("The requested address is not valid in its context. This error is returned if the specified address pointed to by the name parameter is not a valid local IP address on this computer.");
            break;
        case WSAEFAULT:
            bravura_log_err("The system detected an invalid pointer address in attempting to use a pointer argument in a call. This error is returned if the name parameter is NULL, the name or namelen parameter is not a valid part of the user address space, the namelen parameter is too small, the name parameter contains an incorrect address format for the associated address family, or the first two bytes of the memory block specified by name do not match the address family associated with the socket descriptor s.");
            break;
        case WSAEINPROGRESS:
            bravura_log_err("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
            break;
        case WSAEINVAL:
            bravura_log_err("An invalid argument was supplied. This error is returned of the socket s is already bound to an address.");
            break;
        case WSAENOBUFS:
            bravura_log_err("An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full. This error is returned of not enough buffers are available or there are too many connections.");
            break;
        case WSAENOTSOCK:
            bravura_log_err("An operation was attempted on something that is not a socket. This error is returned if the descriptor in the s parameter is not a socket.");
            break;
        }
        return -1;
    }

    if (listen(sock, 200) != 0)
    {
        auto err = WSAGetLastError();
        switch (err)
        {
        case WSANOTINITIALISED:
            bravura_log_err("A successful WSAStartup call must occur before using this function. ");
            break;
        case WSAENETDOWN:
            bravura_log_err("The network subsystem or the associated service provider has failed.");
            break;
        case WSAEADDRINUSE:
            bravura_log_err("The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs during execution of the bind function, but could be delayed until this function if the bind was to a partially wildcard address (involving ADDR_ANY) and if a specific address needs to be committed at the time of this function.");
            break;
        case WSAEINPROGRESS:
            bravura_log_err("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
            break;
        case WSAEINVAL:
            bravura_log_err("The socket has not been bound with bind.");
            break;
        case WSAEISCONN:
            bravura_log_err("Socket is already connected.");
            break;
        case WSAEMFILE:
            bravura_log_err("No more socket descriptors are available.");
            break;
        case WSAENOBUFS:
            bravura_log_err("No buffer space is available.");
            break;
        case WSAENOTSOCK:
            bravura_log_err("The descriptor is not a socket.");
            break;
        case WSAEOPNOTSUPP:
            bravura_log_err("The referenced socket is not of a type that supports the listen operation.");
            break;
        }
        return -1;
    }

    return sock;
}
