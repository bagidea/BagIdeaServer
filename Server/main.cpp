#include <iostream>
#include <cstdlib>
#include <pthread.h>

#include "BISlib/BIS_Server.h"

using namespace std;
using namespace boost;

int serverPort;
int serverMaxUser;

BIS_Server bis;

bool Exit = false;

void* Listen(void*);

void LoginEvent(string userName)
{
	cout << userName << " has login." << endl;
}

void CreateRoomEvent(string userName, string room, int maxUser)
{
	cout << userName << " has create room. - " << room << " : MaxUser(" << maxUser << ")" << endl;
}

void DestroyRoomEvent(string userName, string room)
{
	cout << userName << " has destroy room. - " << room << endl;
}

void* Listen(void* id)
{
	int Online = true;
	int id_ = *(int*)id;
	while(Online)
	{
		if(!bis.ReadMessage(id_))
		{
			if(bis.param.status == DISCONNECT_EVENT)
			{

				cout << bis.param.username << " Disconnect." << endl;
				free(id);
				Online = false;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	bis.LoginEvent = &LoginEvent;
	bis.CreateRoomEvent = &CreateRoomEvent;
	bis.DestroyRoomEvent = &DestroyRoomEvent;

	switch(argc)
	{
		case 2:
			serverPort = atoi(argv[1]);
			cout << "\nServer Port: " << serverPort << endl;
			cout << endl;
			break;
		default:
			cout << "\nServer Port: ";
			cin >> serverPort;
			cout << endl;
	}

	cout << ":::::::::::::::::::: BagIdeaServer ::::::::::::::::::::" << endl << endl;

	cout << "     Create Sock Server: ";
	if(!bis.SocketServer(serverPort))
	{
		cout << "Fail." << endl;
		return 1;
	}
	cout << "Success." << endl;

	cout << "     Bind: ";
	if(!bis.Bind())
	{
		cout << "Fail." << endl;
		return 1;
	}
	cout << "Success." << endl << endl;

	cout << "::::::::::::::::::::: StartServer :::::::::::::::::::::" << endl << endl;

	while(!Exit)
	{
		if(bis.hasConnect())
		{
			pthread_t st;
			cout << "IP: " << bis.param.ip << " has Connected." << endl;
			if(pthread_create(&st, NULL, Listen, (void*)bis.param.id) < 0)
				cout << "Server Error Connection." << endl;
		}
	}

	return 0;
}