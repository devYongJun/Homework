#include "pch.h"
#include <iostream>


using namespace std;

std::mutex Lock;

std::thread RecvThread;

bool Check = true;

char recvBuffer[1024];
int writeCursor = 0;

int GetValidLength(const char* buffer)
{
	int len = 0;
	while (buffer[len] != '\0')
	{
		len++;
	}
	return len;
}

void ResetRecvBuffer()
{
	recvBuffer[0] = 0;
	writeCursor = 0;
}


void RecvFunc(SOCKET _SessionSocket)
{
	while (Check)
	{
		char Buffer[1024];

		int Result = recv(_SessionSocket, Buffer, sizeof(Buffer), 0);

		if (SOCKET_ERROR == Result)
		{
			return;
		}

		int len = GetValidLength(Buffer);
		memcpy(recvBuffer + writeCursor, Buffer, len);
		writeCursor += len;

		if (Buffer[0] == '\0')
		{
			cout << recvBuffer << endl;
			ResetRecvBuffer();
		}
		else
		{
			cout << "패킷 받는중.." << endl;
		}
	}
}

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		std::cout << "WSAStartup Error" << std::endl;
	}

	SOCKET SessionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == SessionSocket)
	{
		return 0;
	}

	std::cout << "IP주소를 입력해주세요 (Q:Loopback, W:Assortrock)" << std::endl;
	std::string Ip;
	std::cin >> Ip;

	if ("Q" == Ip || "q" == Ip)
	{
		Ip = "127.0.0.1";
	}
	else if ("w" == Ip || "W" == Ip)
	{
		Ip = "58.150.30.210";
	}

	SOCKADDR_IN Add = { 0, };
	Add.sin_family = AF_INET;
	Add.sin_port = htons(30001);
	if (SOCKET_ERROR == inet_pton(AF_INET, Ip.c_str(), &Add.sin_addr))
	{
		return 0;
	}
	int Len = sizeof(SOCKADDR_IN);

	if (SOCKET_ERROR == connect(SessionSocket, (const sockaddr*)&Add, Len))
	{
		return 0;
	}

	std::cout << "커넥트 성공." << std::endl;

	RecvThread = std::thread(RecvFunc, SessionSocket);

	while (true)
	{
		// Lock.lock();
		std::cout << "보낼 데이터 입력. (Q:종료)" << std::endl;
		std::string In;
		std::cin >> In;

		if (In == "q" || In == "Q")
		{
			//TCP면 포핸드 쉐이크를 다 깔끔하게 맞춰줍니다.
			closesocket(SessionSocket);
			Check = false;
			RecvThread.join();
			Sleep(1);
			return 0;
		}

		char Buffer[1024] = { "" };

		Sleep(1);

		memcpy_s(Buffer, sizeof(Buffer), In.c_str(), In.size());

		int Result = send(SessionSocket, Buffer, sizeof(Buffer), 0);
	}

	_getch();
}
