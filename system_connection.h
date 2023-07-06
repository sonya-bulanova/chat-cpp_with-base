#pragma once
#include <iostream>

#if defined(_WIN64) // windows
#define PLATFORM_NAME "windows 64-bit"
#include "Windows.h"
#include <winsock2.h>

#elif defined(__linux__) //linux
#define PLATFORM_NAME "linux"

#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

void system_connection();
int ClientChat();
int ChatServer();

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7773 // Будем использовать этот номер порта

static struct sockaddr_in serveraddress, client;
static socklen_t length;
static int socket_file_descriptor, connection, bind_status, connection_status;
