// To use this file with windows, include " -lws2_32 " to your compile statements

#ifndef SOCKET_H
#define SOCKET_H


// Include statements
#include <string>
#include <ctime>

#ifdef _WIN32
#include <winsock2.h>	// Required for server socket connections
#include <windows.h>	// Required for console text colour editing
#else
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define DEFAULT_BUFLEN 1024


namespace Socket {
	void printFormatted(std::string a,std::string b,std::string c) {}
	void print(std::string x) {}
	
	// Configuration struct to hold modifiable parameters
	namespace _internal {
        inline int iFamily = AF_INET;
		inline int iType = 1;
		inline int iProtocol = IPPROTO_TCP;
		inline int boundToPort = -1;
		inline std::string clientResponse = "";
    }


#ifdef _WIN32
#pragma comment(lib,"Ws2_32.lib")

	//This is a stripped down version of the code found at:
	//https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket

	namespace _windows_internal {
		inline WSADATA wsaData = {0};
		inline int iResult;
		inline char recvbuf[DEFAULT_BUFLEN];
		inline int recvbuflen = DEFAULT_BUFLEN;
		inline sockaddr_in service;
		inline SOCKET sock = INVALID_SOCKET;
		inline SOCKET clientSock = INVALID_SOCKET;
	}
	
	
	// Function Definitions
	bool isConnectedToClient() { return (_internal::boundToPort!=-1)&&(_windows_internal::clientSock!=INVALID_SOCKET) ; }
	
	
	// Header-file Definitions
	int StartSocket(int port);
	bool OpenSocket();
	bool CloseClientSocket();
	bool CloseSocket(SOCKET i);
	bool CloseSocket();
	bool BindSocket(int port);
	bool ListenOnSocket();
	bool AcceptClient();
	std::string GetClientResponse();
	bool SendToClient(std::string dataToSend);
	
	
	// Windows Function Definitions
	int StartSocket(int port) {
		printFormatted("SERVER","Info","Starting up server");
		if (!OpenSocket())     { return 1; }
		if (!BindSocket(port)) { return 2; }
		if (!ListenOnSocket()) { return 3; }
		return 0;
	}

	bool OpenSocket() {
		_windows_internal::iResult = WSAStartup(MAKEWORD(2, 2), &_windows_internal::wsaData);
		if (_windows_internal::iResult != 0) {
			printFormatted("SERVER","Error","WSAStartup failed with error: " + std::to_string(_windows_internal::iResult));
			return false;
		}
		
		_windows_internal::sock = socket(_internal::iFamily, _internal::iType, _internal::iProtocol);
		if (_windows_internal::sock != INVALID_SOCKET) {
			printFormatted("SERVER","Success","Successfully opened socket");
			return true;
		}
		
		printFormatted("SERVER","Error","Failed to start socket with error: " + std::to_string(WSAGetLastError()));
		WSACleanup();
		return false;
	}
	
	bool CloseSocket(SOCKET i) {
		_internal::boundToPort = -1;
		_windows_internal::iResult = closesocket(i);
		if (_windows_internal::iResult != SOCKET_ERROR) {
			printFormatted("SERVER","Success","Closed socket");
			return true;
		}
		printFormatted("SERVER","Error","Failed to close socket with error: " + std::to_string(WSAGetLastError()));
		WSACleanup();
		return false;
	}
	
	bool CloseSocket() {
		bool result = CloseSocket(_windows_internal::sock);
		_windows_internal::sock == INVALID_SOCKET;
		return result;
	}
	
	bool CloseClientSocket() {
		bool result = CloseSocket(_windows_internal::clientSock);
		_windows_internal::clientSock == INVALID_SOCKET;
		return result;
	}

	bool BindSocket(int port) {
		if (_windows_internal::sock == INVALID_SOCKET) { return false; }
		
		_windows_internal::service.sin_family = _internal::iFamily;
		_windows_internal::service.sin_addr.s_addr = inet_addr("127.0.0.1");
		_windows_internal::service.sin_port = htons(port);
		
		_windows_internal::iResult = bind(_windows_internal::sock, (SOCKADDR *) &_windows_internal::service, sizeof (_windows_internal::service));
		if (_windows_internal::iResult == SOCKET_ERROR) {
			printFormatted("SERVER","Error","Failed to bind socket to port with error: " + std::to_string(WSAGetLastError()));
			CloseSocket();
			return false;
		}
		
		printFormatted("SERVER","Success","Bound socket to port: " + std::to_string(port));
		_internal::boundToPort = port;
		return true;
	}

	bool ListenOnSocket() {
		if (_internal::iProtocol == IPPROTO_UDP) { printFormatted("SERVER","Warning","UDP connection does not support listening"); return false; }
		if (_windows_internal::sock == INVALID_SOCKET) { return false; }
		if (_internal::boundToPort == -1) { return false; }
		
		if (listen(_windows_internal::sock, SOMAXCONN) == SOCKET_ERROR) {
			printFormatted("SERVER","Error","Could not listen on port: " + std::to_string(_internal::boundToPort));
			print(std::to_string(WSAGetLastError()));
			return false;
		}
		
		printFormatted("SERVER","Log","Listening on socket...");
		return true;
	}

	bool AcceptClient() {
		if (_windows_internal::sock == INVALID_SOCKET) { printFormatted("SERVER","ERROR","Server socket not initialized"); return false; }
		if (_internal::boundToPort == -1) { printFormatted("SERVER","ERROR","Server socket not bound to port"); return false; }
		
		// Try to receive some data, this is a blocking call
		_windows_internal::clientSock = accept(_windows_internal::sock, NULL, NULL);
		if (_windows_internal::clientSock == INVALID_SOCKET) {
			printFormatted("SERVER","Warning","Client accept failed with error: " + std::to_string(WSAGetLastError()));
			CloseSocket(_windows_internal::sock);
			return false;
		} else {
			printFormatted("SERVER","Success","Client connected");
		}
		
		CloseSocket(_windows_internal::sock);
		return true;
	}

	std::string GetClientResponse() {
		if (_windows_internal::clientSock == -1) { printFormatted("SERVER","ERROR","Server socket not initialized"); return "INTERNAL ERROR"; }
		if (_internal::boundToPort == -1) { printFormatted("SERVER","ERROR","Server socket not bound to port"); return "INTERNAL ERROR"; }

		char recvBuff[DEFAULT_BUFLEN] = {0};
		std::string clientResponse;
		long timer = time(NULL);
		while(time(NULL) - timer < 5) {
			int bytesRead = recv(_windows_internal::clientSock, recvBuff, DEFAULT_BUFLEN, 0);
			if (bytesRead > 0) {
				clientResponse += std::string(recvBuff, bytesRead);
			}
			if (_internal::clientResponse.find("\n") != std::string::npos) {
				std::string result = _internal::clientResponse.substr(0, _internal::clientResponse.find("\n"));
				_internal::clientResponse = _internal::clientResponse.substr(_internal::clientResponse.find("\n") + 1, _internal::clientResponse.length() - 1);

				return result;
			}
		}

		return "";
	}

	bool SendToClient(std::string dataToSend) {
		if (_windows_internal::clientSock == INVALID_SOCKET) { printFormatted("SERVER","ERROR","Client socket not initialized"); return false; }
		
		dataToSend += "\n";
		
		char msg[dataToSend.length() + 1];
		strcpy(msg, dataToSend.c_str());
		
		_windows_internal::iResult = send(_windows_internal::clientSock, msg, dataToSend.length(), 0);
		if (_windows_internal::iResult != SOCKET_ERROR) { return true; }
		
		printFormatted("SERVER","ERROR","Client send failed: " + std::to_string(WSAGetLastError()));
		CloseSocket(_windows_internal::clientSock);
		return false;
	}
#endif

#ifdef __linux__
	namespace _linux_internal {
		inline int sock = -1;
		inline int clientSock = -1;
	}
	
	bool isConnectedToClient() { return (_internal::boundToPort!=-1)&&(_linux_internal::clientSock!=INVALID_SOCKET) ; }
	
	int StartSocket(int port) {
		printFormatted("SERVER","Info","Starting up server");
		if (!OpenSocket())     { return -1; }
		if (!BindSocket(port)) { return -2; }
		if (!ListenOnSocket()) { return -3; }
		return 0;
	}

	bool OpenSocket() {
		_linux_internal::sock = socket(iFamily, iType, iProtocol);
		if (_linux_internal::sock < 0) {
			printFormatted("SERVER","Error","Failed to open socket");
		} else {
			printFormatted("SERVER","Success","Successfully opened socket");
		}
		return bool(_linux_internal::sock >= 0);
	}

	bool BindSocket(int port) {
		if (_linux_internal::sock == -1) { return false; }
		
		struct sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = INADDR_ANY;
		sin.sin_port = htons(port);
		
		int result = bind(_linux_internal::sock, (struct sockaddr *) &sin, sizeof sin);
		if (result == 0) {
			boundToPort = port;
			printFormatted("SERVER","Success","Bound socket to port: " + std::to_string(port));
			return 1;
		}

		boundToPort = -1;
		printFormatted("SERVER","Error","Failed to bind to port: " + std::to_string(port));
		CloseSocket();
		return 0;
	}

	bool ListenOnSocket() {
		if (iProtocol == IPPROTO_UDP) { printFormatted("SERVER","Warning","UDP connection does not support listening"); return false; }
		if (_linux_internal::sock == -1) { return false; }
		if (boundToPort == -1) { return false; }
		
		int result = listen(_linux_internal::sock,1);
		if (result == 0) {
			printFormatted("SERVER","Log","Listening on port...");
			return true;
		}

		printFormatted("SERVER","Error","Could not listen on port: " + std::to_string(boundToPort));
		CloseSocket();
		
		return false;
	}

	bool AcceptClient() {
		if (_linux_internal::sock == -1) { printFormatted("SERVER","ERROR","Server socket not initialized"); return false; }
		if (boundToPort == -1) { printFormatted("SERVER","ERROR","Server socket not bound to port"); return false; }
		
		struct sockaddr_in from;
		unsigned int fromlen = sizeof(from);
		_linux_internal::clientSock = accept(_linux_internal::sock, (struct sockaddr *) &from, &fromlen);

		if (_linux_internal::clientSock < 0) {
			printFormatted("SERVER","Warning","Client accept failed");
			return false;
		}

		printFormatted("SERVER","Success","Client connected");
		return true;
	}

	bool CloseSocket() { return CloseSocket(_linux_internal::sock); }
	bool CloseClientSocket() { return CloseSocket(_linux_internal::clientSock); }
	bool CloseSocket(int i) {
		bool result = shutdown(i, 2)==0;
		if (result) { _linux_internal::sock = -1; boundToPort = -1; }
		return result;
	}

	bool SendToClient(std::string dataToSend) {
		if (_linux_internal::clientSock == -1) { printFormatted("SERVER","ERROR","Server socket not initialized"); return false; }
		if (boundToPort == -1) { printFormatted("SERVER","ERROR","Server socket not bound to port"); return false; }
		
		dataToSend += "\n";
		
		const char* sendBuff = dataToSend.c_str();
		send(_linux_internal::clientSock, sendBuff, strlen(sendBuff), MSG_DONTWAIT);
		return false;
	}

	std::string convertToString(char* a) {
		std::string s = a;
		return s;
	}


	std::string GetClientResponse() {
		if (_linux_internal::clientSock == -1) { printFormatted("SERVER","ERROR","Server socket not initialized"); return "INTERNAL ERROR"; }
		if (boundToPort == -1) { printFormatted("SERVER","ERROR","Server socket not bound to port"); return "INTERNAL ERROR"; }
		
		char* sendBuff[DEFAULT_BUFLEN];
		long timer = time(NULL);
		while(time(NULL) - timer < 5) {
			if (recv(_linux_internal::clientSock, sendBuff, DEFAULT_BUFLEN, MSG_DONTWAIT) > 0)
				clientResponse += convertToString((char *)sendBuff);
			if (clientResponse.find("\n") != std::string::npos) {
				std::string result = clientResponse.substr(0,clientResponse.find("\n"));
				clientResponse = clientResponse.substr(
					clientResponse.find("\n")+1,
					clientResponse.length()-1
				);
				
				return result;
			}
		}
		
		return "";
	}
#endif
}
#endif