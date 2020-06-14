#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <thread>
#include <WinInet.h>
#include <tchar.h>
#include <Windows.h>	
#include <fstream>
#include "easendmailobj.tlh"
#include <string>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment(lib, "wininet.lib")
//numero asociado al servidor donde se conectara el cliente 
#define PUERTO_DEFECTO "27015"  
#define BUFLEN_DEFECTO 512

using namespace EASendMailObjLib;
using namespace std;

const int ConnectNormal = 0;
const int ConnectSSLAuto = 1;
const int ConnectSTARTTLS = 2;
const int ConnectDirectSSL = 3;
const int ConnectTryTLS = 4;

int otherF(SOCKET sockEscucha,int resInit,string nomb) {
	//ESCUCHAR EL SOCKET
	//conexiones en cola
	if (listen(sockEscucha, SOMAXCONN) == SOCKET_ERROR) { //somaxconn tiempo max que espera
		cout << "peticion fallida, saliendo..." << WSAGetLastError() << endl;
		closesocket(sockEscucha);
		WSACleanup();
		return 1;
	}

	//ACEPTAR LA CONEXION
	//socket temporal para aceptar conexiones de cliente 
	SOCKET sockCliente;
	sockCliente = INVALID_SOCKET;
	//acepta el socket del cliente
	sockCliente = accept(sockEscucha, NULL, NULL);
	if (sockCliente == INVALID_SOCKET) {
		cout << "conexion fallida" << WSAGetLastError() << endl;
		closesocket(sockEscucha);
		WSACleanup();
		return 1;
	}
	else {
		cout << "\nSe ha conectado un cliente\nid: " << nomb << endl;
	}
	//cout << "Escuchando peticiones del cliente.......\n";
	//aqui ya es posible asignarle un proceso al socket del cliente
	char reciveBuf[BUFLEN_DEFECTO];
	int resEnviado = SOCKET_ERROR;
	int reciveBufLen = BUFLEN_DEFECTO;
	string devolver;
	bool notificacion = false;
	//recive hasta que se termine
	resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
	cout << "conexion desde: " << string(reciveBuf, 0, resInit) << endl;
	cout << "--------------------------------------------------------\n\n";
	cout << "Escuchando peticiones del cliente.......\n";
	do {
		resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
		if (string(reciveBuf, 0, resInit) == "close()") {
			resInit = 0;
		}	
		if (resInit > 0) {
			cout << "Bytes recibidos de " << nomb <<": "<< resInit << endl;
			cout << "Cliente> " <<nomb<<": "<<string(reciveBuf, 0, resInit) << endl;

			if (string(reciveBuf, 0, resInit) == "FTP()") {
				resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
				if (string(reciveBuf, 0, resInit) == "sesion") {
					cout << "--------------------------------------------------------\n";
					cout << "COMENZANDO PROCESO DE ARCHIVOS peticion de: " << nomb << endl;
					do {
						resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
						cout << "comando iniciado: " << string(reciveBuf, 0, resInit) << endl;
						if (string(reciveBuf, 0, resInit) == "FTP_download()") {
							resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
							cout << "El usuario>> " << nomb << " solicita: " << string(reciveBuf, 0, resInit) << endl;
							resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
							if (string(reciveBuf, 0, resInit) == "F") {
								cout << "TRANSACCION FALLIDA DE: " <<nomb<<"\n";
								cout << "Error en GetFile\n\n";
							}
							else if(string(reciveBuf, 0, resInit) == "A"){
								cout << "TRANSACCION COMPLETADA\n\n";
							}
								
						}
						if (string(reciveBuf, 0, resInit) == "FTP_upload()") {
							cout << "--------------------------------------------------------\n";
							cout << "RECIBIENDO DATOS DE: " << nomb << endl;
							resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
							if (string(reciveBuf, 0, resInit) == "exito upload") {
								cout << "1 ARCHIVO HA INGRESADO AL SERVIDOR\n";
								cout << "--------------------------------------------------------\n\n";
							}
							else if (string(reciveBuf, 0, resInit) == "fallo upload") {
								cout << "FALLO EN RECIBIR (1) ARCHIVO(S)\n";
								cout << "--------------------------------------------------------\n\n";
							}
						}
						if (string(reciveBuf, 0, resInit) == "FTP_delete()") {
							resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
							cout << "El usuario: " << nomb << " Comenzo proceso de eliminacion para: "<< string(reciveBuf, 0, resInit) <<endl;
							resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
							if (string(reciveBuf, 0, resInit) == "fallo")
								cout << "ELIMINACION DENEGADA\n";
							if (string(reciveBuf, 0, resInit) == "exito")
								cout << "ELIMINACION COMPLETADA \n";
							cout << "--------------------------------------------------------\n\n";
						}
						if (string(reciveBuf, 0, resInit) == "FTP_mkDirectory()") {
							cout << "CREANDO UN NUEVO DIRECTORIO\n";
							cout << "NUEVO DIRECTORIO EN EL SERVIDOR\n";
						}
					} while (string(reciveBuf, 0, resInit) != "close_FTP()");
					cout << "\n\nPROCESO DE ARCHIVOS FINALIZADO PARA " << nomb << endl;
					cout << "--------------------------------------------------------\n\n";
				}
				else if (string(reciveBuf, 0, resInit) == "fallaFTP") {
					cout << "FALLO EN EL PROCESO DE ARCHIVOS: Error	del tipo InternetConnect\n\n";
					cout << "--------------------------------------------------------\n";
				}

			}
			if (string(reciveBuf, 0, resInit) == "chat()") {
				cout << "EN CHAT CON CLIENTE: "<< nomb<< endl;
				cout << "--------------------------------------------------------\n";
				do {
					resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
					cout << "Cliente> " << nomb << ": " << string(reciveBuf, 0, resInit) << endl;
					if (string(reciveBuf, 0, resInit) != "cerrar_chat()") {
						cout << "msg para: " << nomb << " --> ";
						getline(cin, devolver);		
					}
					else {
						devolver = "HASTA LA PROXIMA :)\n";
					}
					resEnviado = send(sockCliente, devolver.c_str(), devolver.size(), 0);
					if (resEnviado == SOCKET_ERROR) {
						cout << "fallo al enviar: " << WSAGetLastError() << endl;
						closesocket(sockCliente);
						WSACleanup();
						return 1;
					}
					cout << "Bytes enviados: " << resEnviado << endl;
				} while (string(reciveBuf,0,resInit)!= "cerrar_chat()");
				cout << "cliente: " << nomb << " ha abandonado el chat\n";
				cout << "--------------------------------------------------------\n\n";
			}
			if (string(reciveBuf, 0, resInit) == "enviar_correo()") {
				::CoInitialize(NULL);
				cout << "--------------------------------------------------------\n";
				cout << "COMENZANDO PROCESO DE CORREO peticion de: " << nomb <<endl;
				IMailPtr struc_elementos = NULL;
				struc_elementos.CreateInstance(__uuidof(EASendMailObjLib::Mail));
				struc_elementos->LicenseCode = _T("TryIt");
				//email remitente
				resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
				string clienteD = string(reciveBuf, 0, resInit);
				struc_elementos->FromAddr = _T(clienteD.c_str());
				//email destinatario
				resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
				struc_elementos->AddRecipientEx(_T(string(reciveBuf, 0, resInit).c_str()), 0);
				//asunto
				resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
				struc_elementos->Subject = _T(string(reciveBuf, 0, resInit).c_str());
				//mensaje
				resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
				struc_elementos->BodyText = _T(string(reciveBuf, 0, resInit).c_str());
				//direccion smtp del servidor
				struc_elementos->ServerAddr = _T("smtp.gmail.com"); 
				//OPCION DE ARCHIVOS ADJUNTOS
				resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
				if (string(reciveBuf, 0, resInit) != "N") {
					resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
					cout << string(reciveBuf, 0, resInit) << endl;
					if (string(reciveBuf, 0, resInit) == "Imagen()") {
						resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
						//cout << string(reciveBuf, 0, resInit) << endl;
						_bstr_t cid = struc_elementos->AddInline(_T(string(reciveBuf, 0, resInit).c_str()));
					}					
					if (string(reciveBuf, 0, resInit) == "Archivo()") {
						resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
						//cout << string(reciveBuf, 0, resInit) << endl;
						struc_elementos->AddAttachment(_T(string(reciveBuf, 0, resInit).c_str()));
					}						
				}
				//autentificacion xd
				struc_elementos->UserName = _T(clienteD.c_str());
				resInit = recv(sockCliente, reciveBuf, reciveBufLen, 0);
				struc_elementos->Password = _T(string(reciveBuf, 0, resInit).c_str());
				//PUERTO POR EL CUAL VAMOS A ACCEDER
				//puerto determinado con cifrado TLS
				struc_elementos->ServerPort = 587;
				//SSL / TLS
				struc_elementos->ConnectType = ConnectSSLAuto;
				cout << "Enviando mensaje al servidor.....\n\n";
				if (struc_elementos->SendMail() == 0) {
					devolver = "Correo enviado :) :)";
				}
				else {
					devolver = "Ay wey no se pudo enviar el correo :(";
					cout << "ERRR: " << (const TCHAR*)struc_elementos->GetLastErrDescription();
				}
				cout << "\n\nPROCESO DE CORREO FINALIZADO PARA " << nomb <<endl;
				cout << "--------------------------------------------------------\n\n";
				notificacion = true;
				//CONTROLAR LAS NOTIFICACIONES QUE ENVIAN LOS SERVICIOS
				if (notificacion == true) {
					resEnviado = send(sockCliente, devolver.c_str(), devolver.size(), 0);
					notificacion = false;
				}
				else {
					cout << "--> ";
					getline(cin, devolver);
					resEnviado = send(sockCliente, devolver.c_str(), devolver.size(), 0);
				}

				if (resEnviado == SOCKET_ERROR) {
					cout << "fallo al enviar: " << WSAGetLastError() << endl;
					closesocket(sockCliente);
					WSACleanup();
					return 1;
				}
				cout << "Bytes enviados: " << resEnviado << endl;
			}
		}
		else if (resInit == 0) {
			cout << "\ncliente> " << nomb<<" desconectado\n";
		}
		else {
			cout << "fallo en recibir: " << WSAGetLastError() << endl;
			closesocket(sockCliente);
			WSACleanup();
			return 1;
		}
	} while (resInit > 0);

	//DESCONECTAR EL SERVIDOR
	resInit = shutdown(sockCliente, SD_SEND);
	if (resInit == SOCKET_ERROR) {
		cout << "no se puede desconectar :(" << WSAGetLastError() << endl;
		closesocket(sockCliente);
		WSACleanup();
		return 1;
	}
	//limpiar
	closesocket(sockCliente);
}

int main() {
	cout << "comienza servidor" << endl;

	//INICIALIZAR WINSOCK
	WSADATA wsData;
	int resInit;
	struct	addrinfo  *result = NULL, * ptr = NULL, hints; //struct de informacion

	resInit = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (resInit != 0) {
		cout << "fallo en iniciar winsock, saliendo...." << endl;
		return 1;
	}

	//CREAR UN SOCKET PARA EL SERVIDOR
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;			//IPv4 
	hints.ai_socktype = SOCK_STREAM;	//flujo que toma el socket (TCP socket)
	hints.ai_protocol = IPPROTO_TCP;	//protocolo TCP
	hints.ai_flags = AI_PASSIVE;		//usa la dir del socket
	//info de la estructura de la dir del socket
	resInit = getaddrinfo(NULL, PUERTO_DEFECTO, &hints, &result); 

	if (resInit != 0) {
		cout << "fallo en el socket del servidor, saliendo.." << endl;
		WSACleanup();
		return 1;
	}
	// crear un objeto de SOCKET que escuchara las conexiones del cliente
	SOCKET sockEscucha = INVALID_SOCKET; //valor inicial

	sockEscucha = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (sockEscucha == INVALID_SOCKET) {
		cout << "error en el socket: " << WSAGetLastError() << endl;
		freeaddrinfo(result); //libera memoria de getaddrinfo
		WSACleanup();
		return 1;
	}

	//VINCULAR LA RED 
	//unir el socket
	resInit = bind(sockEscucha, result->ai_addr, (int)result->ai_addrlen);//asocia la dir local con un socket
	if (resInit == SOCKET_ERROR) {
		cout << "error en la vinculacion de red" << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(result); //libera info de la func getaddrinfo
	
	while (true) {
		thread t1(otherF, sockEscucha, resInit, "usr 1");
		thread t2(otherF, sockEscucha, resInit, "usr 2");
		//thread t3(otherF, sockEscucha, resInit, "usuario 3");
		t1.join();
		t2.join();
		//t3.join();
	}

	cout << "acaba servidor :)\n";
	WSACleanup();
	return 0;
}