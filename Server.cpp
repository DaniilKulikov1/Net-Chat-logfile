#include<iostream>
#include<string>
#include<WinSock2.h>
#include<WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;


int main()
{
	setlocale(LC_ALL, "ru");

	WSADATA wsaData;

	int iResult(9);

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		cout << "Библиотека не инициализирована" << endl;
		return 1;
	}


	addrinfo *result = nullptr;
	addrinfo *ptr = nullptr;
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;


	iResult = getaddrinfo(nullptr, "27015", &hints, &result);
	if (iResult != 0)
	{
		cout << "Адрес претерпел ошибку" << endl;
		WSACleanup();
		return 1;
	}

	SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "Ошибка сокета" << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}


	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		cout << "Cокет не подсоединился" << endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "Максимальные очереди не выполнены" << endl;
		WSACleanup();
		return 1;
	}

	SOCKET ClientSocket=accept(ListenSocket, nullptr, nullptr);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "Подключение к клиентскому сокету не удалось" << endl;
		closesocket(ListenSocket);
		return 1;
	}

	char recvbuf[1024];
	//int iResult;
	int iSendResult(9);
	int recvbuflen = 1024;

	do
	{
		iResult = recv(ClientSocket, recvbuf, recvbuflen,0);
		if (iResult > 0)
		{
			cout << "Байтов принято: " << iResult;
			cout << "Получено сообщение: " << recvbuf << endl;
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR)
			{
				cout << "Файлы не отосланы" << endl;
				closesocket(ClientSocket);
				WSACleanup();
					return 1;
			}
			cout << "Байтов послано: " << iSendResult << endl;
		}
		else if (iResult == 0)
		{
			cout << "Отправка не удалась" << endl;
		}


	} while (iResult > 0);

	return 0;
}
