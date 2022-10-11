#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include "Winsock2.h"                // заголовок  WS2_32.dll
#pragma comment(lib, "WS2_32.lib")   // экспорт  WS2_32.dll




std::string  SetErrorMsgText(std::string msgText, int code);
std::string GetErrorMsgText(int code);


WSADATA wsaData;	//структура для хранения данных о WSA
SOCKET cS;			//дескриптор сокета клиента
SOCKET sS;			//дескриптор сокета сервера
SOCKADDR_IN serv;	//параметры сокета сервера
SOCKADDR_IN clnt;	//параметры сокета клиента

char ibuf[50], obuf[50];
int libuf = 0, lobuf = 0;


int main()
{
	setlocale(LC_ALL, "Russian");
	std::cout << "Server Started!\n";

	serv.sin_family = AF_INET;				//использовать IP-адресацию
	serv.sin_port = htons(2000);			//порт 2000
	serv.sin_addr.S_un.S_addr = INADDR_ANY;	//любой собственный IP-адрес

	try
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			throw SetErrorMsgText("Sturtup:", WSAGetLastError());
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)	// создание ГВ сокета
			throw SetErrorMsgText("socket:", WSAGetLastError());
		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)		//бинд нового сокета на 2000 TCP
			throw SetErrorMsgText("bind:", WSAGetLastError());
		if (listen(sS, SOMAXCONN) == SOCKET_ERROR)							//прослушивание созданного сокета
			throw SetErrorMsgText("listen", WSAGetLastError());


		int lclnt = sizeof(clnt);
		memset(&clnt, 0, lclnt);
		clock_t start, stop;
		start = clock();

		while (true)
		{
			int i = 0;
			int lcInt = sizeof(clnt);
			if (INVALID_SOCKET == (cS = accept(sS, (sockaddr*)&clnt, &lcInt)))
			{
				std::cout << "Accept: " << WSAGetLastError() << std::endl;
			}

			std::cout << "         Client connect: " << std::endl;
			std::cout << "ADDRES client :          " << inet_ntoa(clnt.sin_addr) << " : " << htons(clnt.sin_port) << std::endl << std::endl << std::endl << std::endl << std::endl;
			while (true)
			{
				int t = clock();

				if (SOCKET_ERROR == recv(cS, ibuf, sizeof(ibuf), NULL))
				{
					std::cout << "Recv: " << WSAGetLastError() << std::endl;
					break;
				}

				std::cout << " Client : ip " << inet_ntoa(clnt.sin_addr) << "	Port: " << htons(clnt.sin_port) << "	Message: "<<" " << ibuf << std::endl;
				

				if (SOCKET_ERROR == send(cS, ibuf, strlen(ibuf) + 1, NULL))
				{
					std::cout << "Send: " << WSAGetLastError() << std::endl;
					break;
				}
				i++;
			}
			std::cout << "\t\tClient Diskonect: " << std::endl;
		}
		stop = clock();
		std::cout << "Время соединения	" << (stop - start)/CLK_TCK;
		//printf("Loop required %f seconds", (stop - start) / CLK_TCK);
		if (closesocket(sS) == SOCKET_ERROR)			//закрываем сокет
			throw SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup", WSAGetLastError());

	}
	catch (std::string errorMsgText) { std::cout << std::endl << errorMsgText; }
	system("pause");
	return 0;
}




std::string GetErrorMsgText(int code)    // cформировать текст ошибки 
{
	std::string msgText;
	switch (code)                      // проверка кода возврата  
	{
	case WSAEINTR:					 msgText = "Работа функции прервана";         break;
	case WSAEACCES:					 msgText = "Разрешение отвергнуто";        break;
	case WSAEFAULT:					 msgText = "Ошибочный адрес";  break;
	case WSAEINVAL:					 msgText = "Ошибка в аргументе";  break;
	case WSAEMFILE:					 msgText = "Слишком много файлов открыто";  break;
	case WSAEWOULDBLOCK:			 msgText = "Ресурс временно недоступен";  break;
	case WSAEINPROGRESS:			 msgText = "Операция в процессе развития";  break;
	case WSAEALREADY:				 msgText = "Операция уже выполняется";  break;
	case WSAENOTSOCK:				 msgText = "Сокет задан неправильно";  break;
	case WSAEDESTADDRREQ:			 msgText = "Требуется адрес расположения";  break;
	case WSAEMSGSIZE:				 msgText = "Сообщение слишком длинное ";  break;
	case WSAEPROTOTYPE:				 msgText = "Неправильный тип протокола для сокета  ";  break;
	case WSAENOPROTOOPT:			 msgText = "Ошибка в опции протокола";  break;
	case WSAEPROTONOSUPPORT:		 msgText = "Протокол не поддерживается ";  break;
	case WSAESOCKTNOSUPPORT:		 msgText = "Тип сокета не поддерживается  ";  break;
	case WSAEOPNOTSUPP:				 msgText = "Операция не поддерживается";  break;
	case WSAEPFNOSUPPORT:			 msgText = "Тип протоколов не поддерживается ";  break;
	case WSAEAFNOSUPPORT:			 msgText = "Тип адресов не поддерживается протоколом ";  break;
	case WSAEADDRINUSE:				 msgText = "Адрес уже используется ";  break;
	case WSAEADDRNOTAVAIL:			 msgText = "Запрошенный адрес не может быть использован ";  break;
	case WSAENETDOWN:				 msgText = "Сеть отключена";  break;
	case WSAENETUNREACH:			 msgText = "Сеть не достижима";  break;
	case WSAENETRESET:				 msgText = "Сеть разорвала соединение";  break;
	case WSAECONNABORTED:			 msgText = "Программный отказ связи ";  break;
	case WSAECONNRESET:				 msgText = "Связь восстановлена  ";  break;
	case WSAENOBUFS:				 msgText = "Не хватает памяти для буферов  ";  break;
	case WSAEISCONN:				 msgText = "Сокет уже подключен  ";  break;
	case WSAENOTCONN:				 msgText = "Сокет не подключен  ";  break;
	case WSAESHUTDOWN:				 msgText = "Нельзя выполнить send: сокет завершил работу  ";  break;
	case WSAETIMEDOUT:				 msgText = "Закончился отведенный интервал  времени";  break;
	case WSAECONNREFUSED:		  	 msgText = "Соединение отклонено  ";  break;
	case WSAEHOSTDOWN:				 msgText = "Хост в неработоспособном состоянии  ";  break;
	case WSAEHOSTUNREACH:			 msgText = "Нет маршрута для хоста   ";  break;
	case WSAEPROCLIM:				 msgText = "Слишком много процессов    ";  break;
	case WSASYSNOTREADY:			 msgText = "Сеть не доступна     ";  break;
	case WSAVERNOTSUPPORTED:		 msgText = "Данная версия недоступна     ";  break;
	case WSANOTINITIALISED:			 msgText = "Не выполнена инициализация WS2_32.DLL     ";  break;
	case WSAEDISCON:				 msgText = "Выполняется отключение";  break;
	case WSATYPE_NOT_FOUND:			 msgText = "Класс не найден ";  break;
	case WSAHOST_NOT_FOUND:			 msgText = "Хост не найден ";  break;
	case WSATRY_AGAIN:				 msgText = "Неавторизированный хост не найден  ";  break;
	case WSANO_RECOVERY:			 msgText = "Неопределенная  ошибка   ";  break;
	case WSANO_DATA:				 msgText = "Нет записи запрошенного типа    ";  break;
	case WSA_INVALID_HANDLE:		 msgText = "Указанный дескриптор события  с ошибкой";  break;
	case WSA_INVALID_PARAMETER:	 	 msgText = "Один или более параметров с ошибкой   ";  break;
	case WSA_IO_INCOMPLETE:			 msgText = "Объект ввода-вывода не в сигнальном состоянии";  break;
	case WSA_IO_PENDING:			 msgText = "Операция завершится позже  ";  break;
	case WSA_NOT_ENOUGH_MEMORY:		 msgText = "Не достаточно памяти   ";  break;
	case WSA_OPERATION_ABORTED:		 msgText = "Операция отвергнута ";  break;
	case WSASYSCALLFAILURE:			 msgText = "Аварийное завершение системного вызова ";  break;
	default:						 msgText = "***ERROR***";      break;
		//case WSAINVALIDPROCTABLE:		 msgText = "Ошибочный сервис  ";  break;
		//case WSAINVALIDPROVIDER:		 msgText = "Ошибка в версии сервиса    ";  break;
		//case WSAPROVIDERFAILEDINIT	 msgText = "Невозможно инициализировать сервис     ";  break;
	};
	return msgText;
};
std::string  SetErrorMsgText(std::string msgText, int code)
{
	return  msgText + "	 " + GetErrorMsgText(code);
};

