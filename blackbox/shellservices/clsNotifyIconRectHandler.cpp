/*
 * clsNotifyIconRect.cpp
 *
 *  Created on: 11 Apr 2009
 *      Author: shaun
 */

#include "clsNotifyIconRectHandler.h"
#include "../../debug/debug.h"

namespace ShellServices
{

#pragma pack(push, 1)
struct IconRectStruct_v1
{
	DWORD dwMagic;
	DWORD dwMessage;
	DWORD cbSize;
	union
	{
		HWND hWnd;
		struct
		{
			DWORD hWndL;
			DWORD hWndH;
		};
	};
	UINT uID;
	GUID guidItem;
};
#pragma pack(pop)

NotifyIconRectHandler::NotifyIconRectHandler()
{
	// TODO Auto-generated constructor stub

}

NotifyIconRectHandler::~NotifyIconRectHandler()
{
	// TODO Auto-generated destructor stub
}

HRESULT NotifyIconRectHandler::ProcessMessage(DWORD p_cbData, PVOID p_lpData)
{
	PRINT("Rect handler");
	IconRectStruct_v1 &data = *reinterpret_cast<IconRectStruct_v1 *>(p_lpData);
	if (data.dwMagic != 0x34753423)
		return 0;
	PRINT("Magic Success");
	switch (data.dwMessage)
	{
	case 1:
	{
		PRINT("Position case");
		POINT mousePos;
		GetCursorPos(&mousePos);
		return MAKELONG(mousePos.x - 5, mousePos.y - 5);
	}
	case 2:
		PRINT("Orientation case");
		return 0x00180018;
	}
}

}
