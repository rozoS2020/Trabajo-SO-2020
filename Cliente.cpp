#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define LONG_BUFFER 512

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <WinInet.h>
#include <conio.h>
#include <urlmon.h>
#include <string>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "Urlmon.lib")
#pragma warning(disable:4996)

void correo(SOCKET sockConexion, string enviar) {
	string corr_remitente, corr_destino, asunto, cuerpo, psw;
	string archivoAD, Img;
	cout << "De: ";
	getline(cin, corr_remitente);
	enviar = send(sockConexion, corr_remitente.c_str(), corr_remitente.size(), 0);
	cout << "Para: ";
	getline(cin, corr_destino);
	enviar = send(sockConexion, corr_destino.c_str(), corr_destino.size(), 0);
	cout << "Asunto: ";
	getline(cin, asunto);
	enviar = send(sockConexion, asunto.c_str(), asunto.size(), 0);
	cout << "Mensaje: ";
	getline(cin, cuerpo);
	enviar = send(sockConexion, cuerpo.c_str(), cuerpo.size(), 0);
	cout << "Adjuntar archivo? [Y/N]: ";
	getline(cin, asunto);

	if (strcmp(asunto.c_str(), "Y") == 0) {
		enviar = send(sockConexion, asunto.c_str(), asunto.size(), 0);
		cout << "##### Imagen() ##### Archivo() #####\n";
		cout << ">>>> ";
		getline(cin, archivoAD);
		if (archivoAD == "Imagen()") {
			enviar = send(sockConexion, archivoAD.c_str(), archivoAD.size(), 0);
			cout << "digite la ruta de la imagen: ";
		}
		else {
			archivoAD = "Archivo()";
			enviar = send(sockConexion, archivoAD.c_str(), archivoAD.size(), 0);
			cout << "digite la ruta del archivo: ";
		}
		getline(cin, archivoAD);
		enviar = send(sockConexion, archivoAD.c_str(), archivoAD.size(), 0);
	}
	else {
		archivoAD = "N";
		enviar = send(sockConexion, archivoAD.c_str(), archivoAD.size(), 0);
	}
	
	cout << "\n\nPOR FAVOR INTRODUZCA SU CLAVE: ";
	//getline(cin, psw);
	char caracter;
	caracter = _getch();
	while (caracter != 13) {
		psw.push_back(caracter);
		cout << "*";
		caracter = _getch();
	}
	enviar = send(sockConexion, psw.c_str(), psw.size(), 0);
}
void menu() {
	cout << "                                         #                                        \n";
	cout << "                                         # ########          #######      ###     \n";
	cout << "      #######  #######  #######  ####### # ##                #   ###        #     \n";
	cout << "      ##   ##  ##   ##      ##   ##   ##   ########          #  ## #        #     \n";
	cout << "      #######  ##   ##     ##    ##   ##         ##  #   #   # ##  #        #     \n";
	cout << "      ##  ##   ##   ##    ##     ##   ##         ##   # #    ###   #        #     \n";
	cout << "      ##   ##  #######  #######  #######   ########    #     #######  #  #######  \n\n";
	cout << "######################################################################################\n";
	cout << "  ###########################################################################\n";
	cout << "  ####                         LISTA DE COMANDOS                         ####\n";
	cout << "  ####  chat()             inicia una conversacion con el servidor       ####\n";
	cout << "  ####  cerrar_chat()      cierra el canal de mensajes con el servidor   ####\n";
	cout << "  ####  enviar_correo()    escriba este comando para enviar un correo    ####\n";
	cout << "  ####  FTP()              inicia un una sesion en FTP                   ####\n";
	cout << "  ####  FTP_download()     descarga un archivo alojado en el servidor    ####\n";
	cout << "  ####  FTP_upload()       dube un archivo al servidor                   ####\n";
	cout << "  ####  FTP_delete()       elimina un archivo                            ####\n";
	cout << "  ####  FTP_mkDirectory()  crea un nuevo directorio                      ####\n";
	cout << "  ####  close_FTP()        cierra la sesion FTP                          ####\n";
	cout << "  ####  descarga_http()    descargar de internet                         ####\n";	
	cout << "  ####  close()            cierra la conexion con el servidor            ####\n";
	cout << "  ####  cls()              limpia la pantalla                            ####\n";
	cout << "  ###########################################################################\n\n";
}
void comandos() {

}
void listarDirectorio(char* dir) {
	WIN32_FIND_DATA findFileData;
	HANDLE          hFind;

	strcat(dir, "\\*");
	wchar_t tw[70];
	std::mbstowcs(tw, dir, strlen(dir) + 1);
	LPWSTR ptr = tw;
	hFind = FindFirstFile(tw, &findFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		std::cout << "Ruta incorrecta";
	else
	{	
		std::cout << findFileData.cFileName << '\n'; //El primer fichero
			// Listamos todos los ficheros restantes
		while (FindNextFile(hFind, &findFileData) != 0)
			std::cout << findFileData.cFileName << '\n';
	}
}


int wmain()
{
	system("color 3F");
	// INICIAR WINSOCK
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	wprintf(L"inicia cliente\n");
	if (iResult != NO_ERROR) {
		wprintf(L"no se pudo iniciar winsock: %d\n", iResult);
		return 1;
	}

	// CREAR UN SOCKET PARA CONECTARNOS AL SERVIDOR 
	SOCKET sockConexion;
	sockConexion = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockConexion == INVALID_SOCKET) {
		wprintf(L"no se pudo crear el socket :(: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// definir ip y por cual puerto se va a conectar 
	sockaddr_in cliente;			//estructura hint
	cliente.sin_family = AF_INET;
	//aqui no se usa inet_addr por la macro de windows no utilizada
	// InetPton(AF_INET, ip como string compatible con PCWSTR, referencia a la dir de la struct);
	
	InetPton(AF_INET, L"192.168.0.7", &cliente.sin_addr.s_addr);
	
	cliente.sin_port = htons(27015);
	
	// CONECTAR CON EL SERVIDOR 
	iResult = connect(sockConexion, (SOCKADDR*)& cliente, sizeof(cliente));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"conexion fallida: %ld\n", WSAGetLastError());
		iResult = closesocket(sockConexion);
		if (iResult == SOCKET_ERROR)
			wprintf(L"fallo en cerrar el socket: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	else {
		wprintf(L"               CONEXION ESTABLECIDA CON EL SERVIDOR rozoS v0.1\n\n");
		menu();
	}

	// variables para enviar los bytes
	int longBuffer = LONG_BUFFER;
	char bufArr[LONG_BUFFER] = "";	
	char *dir_envBuffer;
	dir_envBuffer = bufArr;
	string env;
	int enviar;
	env = "192.168.0.7";
	enviar = send(sockConexion, env.c_str(), env.size(), 0);
	//ENVIAR BYTES 
	do {
		cout << "--> ";
		getline(cin,env);
		enviar = send(sockConexion, env.c_str(), env.size(), 0);
		if (iResult == SOCKET_ERROR) {
			cout << "no se pudo enviar los bytes :(" << WSAGetLastError() << endl;
			WSACleanup();
			return 1;
		}
		if (strcmp(env.c_str(), "enviar_correo()") == 0) {
			correo(sockConexion,env);
			iResult = recv(sockConexion, bufArr, longBuffer, 0);
			if (iResult == 0) {
				cout << "conexion cerrada\n";
			}
			else
			{
				cout << "\nServidor > " << string(bufArr, 0, iResult) << endl;
			}
		}
		if (strcmp(env.c_str(), "chat()") == 0) {
			do {
				cout << "--> ";
				getline(cin, env);
				enviar = send(sockConexion, env.c_str(), env.size(), 0);
				if (iResult == SOCKET_ERROR) {
					cout << "no se pudo enviar el mensaje :( codigo error:" << WSAGetLastError() << endl;
					WSACleanup();
					return 1;
				}
				iResult = recv(sockConexion, bufArr, longBuffer, 0);
				cout << "Servidor > " << string(bufArr, 0, iResult) << endl;
			} while (strcmp(env.c_str(), "cerrar_chat()")!=0);
			cout << "chat cerrado\n";
		}
		if (strcmp(env.c_str(), "cls()") == 0) {
			system("cls");
			menu();
		}
		//descargar archivo de la web
		if (strcmp(env.c_str(),"descarga_http()")==0) {
			string url,nomArch;
			LPCWSTR ur,nom;
			cout << "Digita la url>>>  ";
			getline(cin, url);
			std::wstring stemp = std::wstring(url.begin(),url.end());
			ur = stemp.c_str();
			getline(cin,nomArch);
			URLDownloadToFile(0, ur,
				L"C:\\Users\\santi\\Desktop\\"+nomArch+".exe", 0, 0);
			env = "DESCARGA COMPLETADA";
			enviar = send(sockConexion, env.c_str(), env.size(), 0);
			cout << "se ha descargado :)\n\n";
		}
		//  FTP()
		if (strcmp(env.c_str(), "FTP()") == 0) {
			if (::InternetAttemptConnect(0) != ERROR_SUCCESS) {
				cout << "falla internetattempconnect\n";
				return 1;
			}
			bool check;

			//HABILITAR FUNCIONALIDAD 
			HINTERNET hInternet, hConexion;

			hInternet = ::InternetOpenA("192.168.0.7", INTERNET_OPEN_TYPE_DIRECT,
				NULL, NULL, INTERNET_FLAG_CACHE_ASYNC);
			if (hInternet == NULL) {
				cout << "Error en InternetOpenA: " << GetLastError() << endl;
				return 1;
			}
			//CREDENCIALES PARA INTERCONNECT :v besto constraseña :v :v
			//contraseña = usr
			string PswUsr;
			cout << "contraseña: ";
			//getline(cin, PwsUsr);
			char caracter;
			caracter = _getch();
			while (caracter != 13) {
				PswUsr.push_back(caracter);
				cout << "*";
				caracter = _getch();
			}
			//CONECTAR
			hConexion = ::InternetConnectA(hInternet, "192.168.0.7", 21,
				"usr 1", PswUsr.c_str(),
				INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

			if (hConexion == NULL) {
				cout << "  CLAVE INCORRECTA / VERIFIQUE SUS CREDENCIALES\n";
				//return 1;
				env = "fallaFTP";
				enviar = send(sockConexion, env.c_str(), env.size(), 0);
			}
			else {
				cout << "\n############ BIENVENIDO AL REPOSITORIO DE ARCHIVOS DE rozoS v0.1 ############\n\n";
				env = "sesion";
				enviar = send(sockConexion, env.c_str(), env.size(), 0);

				// ACCIONES QUE EL USR PIDA :V
				string comandoFTP;
				do
				{
					cout << ">>>>> ";
					getline(cin, comandoFTP);
					enviar = send(sockConexion, comandoFTP.c_str(), comandoFTP.size(), 0);
					if (strcmp(comandoFTP.c_str(), "FTP_download()") == 0) {
						string Archivo, newArch, tipoA;
						cout << "archivo>>>>> ";
						getline(cin, Archivo);

						cout << "Formatos ### txt ### word ### excel ### pdf ### zip ### png ###\n";
						getline(cin, tipoA);
						if (strcmp(tipoA.c_str(), "txt") == 0) 
							Archivo = Archivo + ".txt";
						if(strcmp(tipoA.c_str(),"word")==0)
							Archivo = Archivo + ".docx";
						if (strcmp(tipoA.c_str(), "excel") == 0)
							Archivo = Archivo + ".xlsx";
						if (strcmp(tipoA.c_str(), "pdf") == 0)
							Archivo = Archivo + ".pdf";
						if (strcmp(tipoA.c_str(), "zip") == 0)
							Archivo = Archivo + ".rar";
						if (strcmp(tipoA.c_str(), "png") == 0)
							Archivo = Archivo + ".png";
						//archivo que solicita
						enviar = send(sockConexion, Archivo.c_str(), Archivo.size(), 0);
						cout << "nombre del nuevo archivo>>>>> ";
						getline(cin, newArch);
						if (strcmp(tipoA.c_str(), "txt") == 0)
							newArch = "C:\\Users\\santi\\Desktop\\" + newArch + ".txt";
						if (strcmp(tipoA.c_str(), "word") == 0)
							newArch = "C:\\Users\\santi\\Desktop\\" + newArch + ".docx";
						if (strcmp(tipoA.c_str(), "excel") == 0)
							newArch = "C:\\Users\\santi\\Desktop\\" + newArch + ".xlsx";
						if (strcmp(tipoA.c_str(), "pdf") == 0)
							newArch = "C:\\Users\\santi\\Desktop\\" + newArch + ".pdf";
						if (strcmp(tipoA.c_str(), "zip") == 0)
							newArch = "C:\\Users\\santi\\Desktop\\" + newArch + ".rar";
						if (strcmp(tipoA.c_str(), "png") == 0)
							newArch = "C:\\Users\\santi\\Desktop\\" + newArch + ".png";
						

						check = ::FtpGetFileA(hConexion, Archivo.c_str(),
							newArch.c_str(), true, FILE_ATTRIBUTE_NORMAL,
							INTERNET_FLAG_TRANSFER_BINARY, 0);
						// error 5 permiso denegado
						//error 80 el archivo ya existe
						if (check == false) {
							cout << "DESCARGA FALLIDA :(\n";
							comandoFTP = "F";
							enviar = send(sockConexion, comandoFTP.c_str(), comandoFTP.size(), 0);
							cout << "Error en GetFile: " << GetLastError() << endl;
							//return 1;
						}
						else {
							comandoFTP = "A";
							enviar = send(sockConexion, comandoFTP.c_str(), comandoFTP.size(), 0);
							cout << "ARCHIVO DESCARGADO :)\n";
						}
					}
					if (strcmp(comandoFTP.c_str(), "FTP_upload()") == 0) {
						string ruta,nameSer;
						cout << "ruta del archivo>>>>> ";
						getline(cin, ruta);
						cout << "nombre en el servidor>>>>> ";
						getline(cin, nameSer);
						check = FtpPutFileA(hConexion, ruta.c_str(), nameSer.c_str(),
							FTP_TRANSFER_TYPE_ASCII,0);
						if (check == false) {
							cout << "No se pudo subir el archivo\n";
							comandoFTP = "fallo upload";
							enviar = send(sockConexion, comandoFTP.c_str(), comandoFTP.size(), 0);
						}
						else {
							cout << "Archivo subido con exito\n";
							comandoFTP = "exito upload";
							enviar = send(sockConexion, comandoFTP.c_str(), comandoFTP.size(), 0);
						}
					}
					if (strcmp(comandoFTP.c_str(), "FTP_delete()") == 0) {
						cout << "Archivo a eliminar: ";
						getline(cin, comandoFTP);
						
						enviar = send(sockConexion, comandoFTP.c_str(), comandoFTP.size(), 0);
						cout << "Confirma eliminar este archivo? [Y:si | N:no]: ";
						getline(cin, env);
						if (env == "Y") {
							check = FtpDeleteFileA(hConexion, comandoFTP.c_str());
							if (check == false) {
								cout << "No se pudo eliminar el archivo\n";
								comandoFTP = "fallo";
								enviar = send(sockConexion, comandoFTP.c_str(), comandoFTP.size(), 0);
							}
							else {
								cout << "Archivo eliminado\n";
								comandoFTP = "exito";
								enviar = send(sockConexion, comandoFTP.c_str(), comandoFTP.size(), 0);
							}
						}
						else if (env == "N") {
							cout << "\n\n";
						}

					}
					if (strcmp(comandoFTP.c_str(), "FTP_list()") == 0) {
						char* dir = new char[MAX_PATH];
						string s = "C:\\Users\\santi\\Desktop\\python";
						dir = (char *)s.c_str();
						listarDirectorio(dir);
					}
					if (strcmp(comandoFTP.c_str(), "FTP_mkDirectory()") == 0) {
						cout << "nombre del directorio: ";
						getline(cin,comandoFTP);
						check = FtpCreateDirectoryA(hConexion, comandoFTP.c_str());
					}
				} while (strcmp(comandoFTP.c_str(), "close_FTP()") != 0);
				

			}
			
		}
	} while (strcmp(env.c_str(),"close()") != 0);

	//CERRAR CONEXION
	iResult = closesocket(sockConexion); //retorna 0
	if (iResult == SOCKET_ERROR) {
		wprintf(L"fallo en cerrar el socket: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	system("pause");
	WSACleanup();
	return 0;
}
