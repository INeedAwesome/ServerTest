#include "stdafx.h"
#include "Server.h"

Server::Server()
{
}

Server::~Server()
{
}

void Server::Run()
{
	this->Init();
	this->Terminate();
}

bool Server::Init()
{
	int res = 0;
	printf("Server starting...\n");
	if (!this->ServerInit())
		return false;
	if (!this->ResolveServer())
		return false;
	if (!this->CreateSocket())
		return false;
	if (!this->BindSocket())
		return false;

	printf("Server started!\n");
	
	if (!this->SearchConnection())
		return false;
	
	printf("Found a connection!\n");

	this->SendAndReceiveData();
}

bool Server::SendAndReceiveData()
{
	// Receive until the peer shuts down the connection
	int sendRes = 0;
	int amountBytesRecv = 1;
	m_Running = true;
	while (m_Running) {

		amountBytesRecv = recv(m_ClientSocket, m_Recvbuf, m_Recvbuflen, 0); // if success, amountBytesRecv is amount of bytes received from the client
		if (amountBytesRecv > 0) 
		{
			// we got something from the client.

			this->HandleFromClient(m_Recvbuf, amountBytesRecv);
		}
		else
		{
			// we didnt get something from the client.
		}
	}
	return true;
}

void Server::HandleFromClient(char buffer[DEFAULT_BUFLEN], int incomingBytes)
{
	printf("Buffer: %s, Bytes received: %d\n", buffer, incomingBytes);
	
	if (buffer)
	{
		std::string buff(buffer);

		std::string c_Close = "close";

		if (buff.find(c_Close) != std::string::npos)
			m_Running = false;
	}

	

}

bool Server::ServerInit()
{
	int res = 0;
	res = WSAStartup(MAKEWORD(2, 2), &m_WsaData);
	if (res != 0)
	{
		printf("WSAStartup failed: %d\n", res);
		return false;
	}

	ZeroMemory(&m_AddrinfoHints, sizeof(m_AddrinfoHints));
	{
		m_AddrinfoHints.ai_family = AF_INET;
		m_AddrinfoHints.ai_socktype = SOCK_STREAM;
		m_AddrinfoHints.ai_protocol = IPPROTO_TCP;
		m_AddrinfoHints.ai_flags = AI_PASSIVE;
	}
	return true;
}

bool Server::ResolveServer()
{
	int res = 0;
	res = getaddrinfo(NULL, DEFAULT_PORT, &m_AddrinfoHints, &m_AddrinfoResult);
	if (res != 0)
	{
		printf("getaddrinfo failed: %d\n", res);
		WSACleanup();
		return false;
	}
	return true;
}

bool Server::CreateSocket()
{
	m_ListenSocket = INVALID_SOCKET;
	m_ListenSocket = socket(m_AddrinfoResult->ai_family, m_AddrinfoResult->ai_socktype, m_AddrinfoResult->ai_protocol);
	if (m_ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed: %ld\n", WSAGetLastError());
		freeaddrinfo(m_AddrinfoResult);
		WSACleanup();
		return false;
	}
	return true;
}

bool Server::BindSocket()
{
	int res = 0;
	res = bind(m_ListenSocket, m_AddrinfoResult->ai_addr, (int)m_AddrinfoResult->ai_addrlen);
	if (res == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(m_AddrinfoResult);
		closesocket(m_ListenSocket);
		WSACleanup();
		return false;
	}
	freeaddrinfo(m_AddrinfoResult);
	return true;
}

bool Server::SearchConnection()
{
	int res = 0;
	printf("Listening for a connection...\n");

	res = listen(m_ListenSocket, SOMAXCONN);
	if (res == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(m_ListenSocket);
		WSACleanup();
		return false;
	}

	m_ClientSocket = accept(m_ListenSocket, 0, 0);

	// close, since we dont need it anymore.
	closesocket(m_ListenSocket);
	return true;
}

void Server::Terminate()
{
	// shutdown the connection since we're done
	shutdown(m_ClientSocket, SD_BOTH); // Don't care about result since we're closing.

	if (m_ListenSocket)
		closesocket(m_ListenSocket);
	if (m_ClientSocket)
		closesocket(m_ClientSocket);
	WSACleanup();
}
