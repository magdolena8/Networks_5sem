﻿#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <atlbase.h>
#include <ctime>
#include <atlconv.h>
#include <sstream>
#include "Winsock2.h"                // заголовок  WS2_32.dll
#pragma comment(lib, "WS2_32.lib")   // экспорт  WS2_32.dll

std::string GetErrorMsgText(int code);
std::string  SetErrorMsgText(std::string msgText, int code);
std::string SetMailslotError(std::string msgText, int code);

int main()
{
	setlocale(LC_ALL, "Rus");

	HANDLE hMailSlot; // дескриптор канала
	DWORD readBytes;
	unsigned int start, stop;
	char mailSlotName[5];
	std::string slotName = "\\\\.\\mailslot\\";
	char rbuf[500];


	try
	{
		std::cout << "Enter server name\t\t";
		std::cin >> mailSlotName;
		slotName += mailSlotName;
		if ((hMailSlot = CreateMailslot(std::wstring(slotName.begin(), slotName.end()).c_str(),
			500,	//message length
			180000,  //180 000ms = 3min
			NULL
		)) == INVALID_HANDLE_VALUE)
			throw SetMailslotError("createMailslot", GetLastError());



		for (int i = 0; i < 1000; i++) {
			if (!ReadFile(hMailSlot, rbuf, sizeof(rbuf), &readBytes, NULL)) {
				if (GetLastError() == 121) {
					std::cout << "Timeout expired\n\n";
					throw SetMailslotError("readfile", GetLastError());
				}
				else throw SetMailslotError("readFile", GetLastError());
			}
			else start = clock();
			std::cout << "New message\t\t" << rbuf << std::endl;
		}
		stop = clock();
		std::cout << "\n\nTime spent:\t\t" << stop - start << "\n";
		CloseHandle(hMailSlot);
	}
	catch (std::string ErrorMailSlotText)
	{
		std::cout << std::endl << ErrorMailSlotText;
	}
	system("pause");
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
	default:						 msgText = "***ERROR***\nCode: " + std::to_string(code) + "\n";      break;
	};
	return msgText;
};
std::string  SetErrorMsgText(std::string msgText, int code)
{
	return  msgText + "	 " + GetErrorMsgText(code);
};
std::string SetMailslotError(std::string msgText, int code) {
	return  msgText + "	 " + GetErrorMsgText(code);
}