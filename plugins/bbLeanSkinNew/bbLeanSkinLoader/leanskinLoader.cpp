/***************************************************************************
 *   Copyright (C) 2008 by   *
 *     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <iostream>
#include <stdio.h>
#include <windows.h>

using std::cout;
using std::cin;
using std::endl;

HANDLE hRdSlave,hWrSlave;

HWND hLoaderWnd;

#define WM_BBMESSAGE WM_APP+1
#define WM_DEADPIPE WM_APP+2

DWORD WINAPI PipeReadThread(LPVOID args)
{
	char message[1025];
	char *messageStart;
	DWORD numRead;
	while (1)
	{
		if (!ReadFile(hRdSlave,&message,1024,&numRead,NULL))
		{
			PostMessage(hLoaderWnd,WM_DEADPIPE,NULL,NULL);
			cout<<"Read problem..."<<endl;
			return 0;
		}
		messageStart=message;
		do
		{
			cout<<"We got : \""<<messageStart<<"\""<<endl;
			SendMessage(hLoaderWnd,WM_BBMESSAGE,(WPARAM)messageStart,NULL);
			if (!strcmp(messageStart,"DIE"))
			{
				cout<<"Thread Terminating!!!!"<<endl;
				return 0;
			}
			messageStart+=strlen(messageStart)+1;
		}
		while ((messageStart-message)<numRead);

	}
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc!=3)
	{
		cout<<"Incorrect number of arguments used (require 2)"<<endl;
		return 1;
	}
	cout<<argc<<endl;
	for (int i=1;i<argc;++i)
	{
		if (!strncmp(argv[i],"--inpipe=",9))
		{
			cout<<(argv[i]+9)<<endl;
			hRdSlave=(HANDLE)strtol(argv[i]+9,NULL,0);
		}
		else if (!strncmp(argv[i],"--outpipe=",10))
		{
			hWrSlave=(HANDLE)strtol(argv[i]+10,NULL,0);
		}
	}
	cout<<"Input pipe : "<<hRdSlave<<endl;
	cout<<"Output pipe : "<<hWrSlave<<endl;
	cout<<"Starting Read thread..."<<endl;
	DWORD threadID;
	HANDLE threadHandle;
	threadHandle=CreateThread(NULL,0,PipeReadThread,NULL,0,&threadID);
	WaitForSingleObject(threadHandle,INFINITE);
	cout<<"Our reader thread died, terminating"<<endl;
	return 0;
}
