#include <iostream>
#include <charconv>
#include <string>
#include "Winsock2.h"                // заголовок  WS2_32.dll
#pragma comment(lib, "WS2_32.lib")   // экспорт  WS2_32.dll
#include "WS2tcpip.h"
#pragma warning(disable : 4996)


std::string  SetErrorMsgText(std::string msgText, int code);
std::string GetErrorMsgText(int code);
bool GetServer(char* call, short port, struct sockaddr_in& from, int& flen);
bool  GetServerByName(char* name, char* call, struct sockaddr_in& from, int& flen);


WSADATA wsaData;	//структура для хранения данных WSA
SOCKET cC;			//дескриптор сокета клиента
SOCKET cS;			//дескриптор сокета сервера
SOCKADDR_IN clnt;	//параметры сокета клиента
SOCKADDR_IN serv;	//параметры сокета сервера
int lenserv = sizeof(serv);

char ibuf[50];
int libuf = 0, lobuf = 0;



int main()
{
	setlocale(LC_ALL, "Russian");
	std::cout << "Client Started!\n";

	//serv.sin_family = AF_INET;						//использовать IP-адресацию
	//serv.sin_port = htons(2000);					//порт 2000
	//inet_pton(AF_INET, "127.0.0.1", &(serv.sin_addr)); //Server adress	

	memset(&clnt, 0, sizeof(clnt));

	try {
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			throw SetErrorMsgText("Sturtup:", WSAGetLastError());
		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)	//создание сокета
			throw SetErrorMsgText("socket:", WSAGetLastError());

		char resTimeout[5] = "1000";
		setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, resTimeout, sizeof(resTimeout));

		char serverName[18] = "DESKTOP-B9F1C60";
		char call[4];
		std::cin >> call;

		GetServerByName(serverName, call, serv, lenserv);
		//DESKTOP - 5LST8PB
		//GetServer(call, 2000, serv, lenserv);


		if (closesocket(cC) == SOCKET_ERROR)			//закрываем сокет
			throw SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup", WSAGetLastError());
	}
	catch (std::string errorMsgText) { std::cout << std::endl << errorMsgText << "\n"; }
	system("pause");
	return 0;
}





bool GetServer(
	char* call, //[in] позывной сервера  
	short            port, //[in] номер порта сервера    
	struct sockaddr_in& from, //[out] указатель на SOCKADDR_IN
	int& flen  //[out] указатель на размер from 
)
{
	from.sin_family = AF_INET;               // используется IP-адресация  
	from.sin_port = htons(port);             // порт 2000
	from.sin_addr.s_addr = INADDR_BROADCAST; // всем 
	int lenobuf;
	char servAnswer[50];

	int optval = 1;
	if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
		throw  SetErrorMsgText("setsockopt:", WSAGetLastError());

	if ((lenobuf = sendto(cC, call, sizeof(call), NULL, (sockaddr*)&from, sizeof(from))) == SOCKET_ERROR)
		throw  SetErrorMsgText("sendto_all:", WSAGetLastError());
	if (libuf = recvfrom(cC, servAnswer, sizeof(servAnswer), NULL, (sockaddr*)&from, &flen) == SOCKET_ERROR)
		throw SetErrorMsgText("recvfrom:", WSAGetLastError());
	if (strcmp(call, servAnswer) == 0) {
		std::cout << "\nConnected\n";
		std::cout << inet_ntoa(from.sin_addr) << "\n";
		std::cout << from.sin_port << "\n\n";

		return true;
	}
	else return false;

	return true;
}





bool  GetServerByName(
	char* name, //[in] имя компьютера в сети  
	char* call, //[in] позывной   
	struct sockaddr_in& from, //[in,out] указатель на SOCKADDR_IN
	int& flen  //[in,out] указатель на размер from 
)
{
	try
	{
		char servAnswer[50];
		from.sin_family = AF_INET;               // используется IP-адресация  
		from.sin_port = htons(2000);             // порт 2000
		from.sin_addr.s_addr = inet_addr(inet_ntoa(*(in_addr*)(gethostbyname(name)->h_addr)));
		//from.sin_addr.s_addr = inet_addr(gethostbyname(name)->h_addr);
		//inet_pton(AF_INET, gethostbyname(name), &(serv.sin_addr)); //Server adress	
		if (sendto(cC, call, sizeof(call), NULL, (const sockaddr*)&from, flen) == SOCKET_ERROR) {
			throw SetErrorMsgText("sendto:", WSAGetLastError());
		}


		if (libuf = recvfrom(cC, servAnswer, sizeof(servAnswer), NULL, (sockaddr*)&from, &flen) == SOCKET_ERROR)
			throw SetErrorMsgText("recvfrom:", WSAGetLastError());
		if (strcmp(call, servAnswer) == 0) {
			std::cout << "\nConnected\n";
			std::cout << inet_ntoa(from.sin_addr) << "\n";
			std::cout << from.sin_port << "\n\n";
			return true;
		}
	}
	catch (const std::string errorMsgText)
	{
		std::cout << errorMsgText;
		return false;
	}
	return false;

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

