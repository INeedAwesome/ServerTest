#pragma once

#include <iostream>

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class Server
{
public:
	Server();
	~Server();

	void Run();
	bool Init();

	bool ServerInit();
	bool ResolveServer();
	bool CreateSocket();
	bool BindSocket();
	bool SearchConnection();

	bool SendAndReceiveData();

	void HandleFromClient(char buffer[DEFAULT_BUFLEN], int incomingBytes);

	void Terminate();
private:
	bool m_Running = true;

	WSAData m_WsaData = {};

	struct addrinfo* m_AddrinfoResult = NULL;
	struct addrinfo* m_AddrinfoPtr = NULL;
	struct addrinfo m_AddrinfoHints;

	char m_Recvbuf[DEFAULT_BUFLEN];
	int m_Recvbuflen = DEFAULT_BUFLEN;

	SOCKET m_ListenSocket;

	SOCKET m_ClientSocket;
};

