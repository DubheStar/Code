#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

WCHAR* get_local_ip(const WCHAR* szHostName);
