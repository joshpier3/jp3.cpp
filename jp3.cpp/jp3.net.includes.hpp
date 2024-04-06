#ifndef JP3_NET_INCLUDES_HPP
#define JP3_NET_INCLUDES_HPP

#if __has_include(<Winsock2.h>)

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT 0x0A00
#include <WinSock2.h>
#include <WS2tcpip.h>

#ifndef WINSOCK_VERSION
#define WINSOCK_VERSION MAKEWORD(2, 2)
#endif//WINSOCK_VERSION

#ifndef JP3_NET_BUILD_WINDOWS
#define JP3_NET_BUILD_WINDOWS
#endif//JP3_NET_BUILD_WINDOWS

#else//__has_include(<Winsock2.h>)

//TODO: Add linux and mac includes here as well.
//Help from the community maybe?

#endif//__has_include(<Winsock2.h>)

#endif//JP3_NET_INCLUDES_HPP