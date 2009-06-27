/*
 * clsNotifyIconSrv.cpp
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#include "clsNotifyIconSrv.h"
#include "clsNotifyIconHandler.h"

#include "clsServiceRegistrar.h"
#include "clsServiceManager.h"

#include <vector>
#include <typeinfo>

#define SERVICE_NAME "SRV_NotifyIcon"

std::map<ATOM,ShellServices::eNotificationIconInfo> g_trayInfoMapping;

void InitTrayMapping()
{
	g_trayInfoMapping[AddAtom("TrayIcon::Window")] = ShellServices::NI_HWND;
	g_trayInfoMapping[AddAtom("TrayIcon::ID")] = ShellServices::NI_ID;
	g_trayInfoMapping[AddAtom("TrayIcon::CallbackMessage")] = ShellServices::NI_CALLBACKMESSAGE;
	g_trayInfoMapping[AddAtom("TrayIcon::Icon")] = ShellServices::NI_ICON;
	g_trayInfoMapping[AddAtom("TrayIcon::AnsiTip")] = ShellServices::NI_TIP;
	g_trayInfoMapping[AddAtom("TrayIcon::UnicodeTip")] = ShellServices::NI_TIP;
	g_trayInfoMapping[AddAtom("TrayIcon::AnsiInfoText")] = ShellServices::NI_INFOTEXT;
	g_trayInfoMapping[AddAtom("TrayIcon::UnicodeInfoText")] = ShellServices::NI_INFOTEXT;
	g_trayInfoMapping[AddAtom("TrayIcon::AnsiInfoTitle")] = ShellServices::NI_INFOTITLE;
	g_trayInfoMapping[AddAtom("TrayIcon::UnicodeInfoTitle")] = ShellServices::NI_INFOTITLE;
	g_trayInfoMapping[AddAtom("TrayIcon::InfoIcon")] = ShellServices::NI_INFOICON;
	g_trayInfoMapping[AddAtom("TrayIcon::InfoFlags")] = ShellServices::NI_INFOFLAGS;
	g_trayInfoMapping[AddAtom("TrayIcon::InfoTimout")] = ShellServices::NI_INFOTIMEOUT;
	g_trayInfoMapping[AddAtom("TrayIcon::Version")] = ShellServices::NI_VERSION;
	g_trayInfoMapping[AddAtom("TrayIcon::Legacy")] = ShellServices::NI_LEGACY;
}

namespace ShellServices
{

NotifyIconSrv::NotifyIconSrv():
		Service(SERVICE_NAME),
		m_imp(NULL),
		m_iconFactory(NULL),
		m_useProxy(false)
{
	m_iconFactoryProp = AddAtomA("NI_IconFactory");
	m_useProxyProp = AddAtomA("NI_UseProxy");
	m_callbackAddProp = AddAtomA("NI_Callback_Add");
	m_callbackModProp = AddAtomA("NI_Callback_Mod");
	m_callbackDelProp = AddAtomA("NI_Callback_Del");
	m_cleanTrayCommand = AddAtomA("NI_CleanTray");
	m_getNotificationIconInfoFn = RegisterAtom("FN_GetNotificationIconInfo");
	m_lookupIconFn = RegisterAtom("FN_LookupIcon");
	m_GetTraySizeFn = RegisterAtom("FN_GetTraySize");
}

NotifyIconSrv::~NotifyIconSrv()
{
	DeleteAtom(m_iconFactoryProp);
	DeleteAtom(m_useProxyProp);
	DeleteAtom(m_callbackAddProp);
	DeleteAtom(m_callbackModProp);
	DeleteAtom(m_callbackDelProp);
	DeleteAtom(m_cleanTrayCommand);
}

bool NotifyIconSrv::_SetProperty(ATOM p_property, PVOID p_value)
{
	if (p_property == m_iconFactoryProp)
	{
		m_iconFactory = p_value;
		return true;
	}
	else if (p_property == m_useProxyProp)
	{
		m_useProxy = p_value;
		return true;
	}
	else if (p_property == m_callbackAddProp)
	{
		if (m_imp)
		{
			m_imp->RegisterCallback(TCALLBACK_ADD, reinterpret_cast<NotificationIconCallback>(p_value));
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (p_property == m_callbackModProp)
	{
		if (m_imp)
		{
			m_imp->RegisterCallback(TCALLBACK_MOD, reinterpret_cast<NotificationIconCallback>(p_value));
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (p_property == m_callbackDelProp)
	{
		if (m_imp)
		{
			m_imp->RegisterCallback(TCALLBACK_DEL, reinterpret_cast<NotificationIconCallback>(p_value));
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool NotifyIconSrv::_Start()
{
	if (!m_imp)
	{
		PRINT("Starting NotifyIcon service");
		m_imp = new NotifyIconHandler(reinterpret_cast<LegacyNotficationIconFactory>(m_iconFactory), m_useProxy);
		s_serviceManager->Call("SRV_ShellTrayWnd", "FN_SetHandler", Arg<UINT>(1), Arg<ShellServiceHandler *>(m_imp));
		InitTrayMapping();
		return true;
	}
	else
	{
		return false;
	}
}

bool NotifyIconSrv::_Stop()
{
	if (m_imp)
	{
		PRINT("Stopping NotifyIcon service");
		delete m_imp;
		m_imp = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

bool NotifyIconSrv::Call(ATOM p_command)
{
	if (p_command == m_cleanTrayCommand)
	{
		m_imp->CleanTray();
		return true;
	}
	return false;
}

bool NotifyIconSrv::GetNotificationIconInfo(NotificationIcon *p_icon, PVOID p_return[], ATOM p_info[], UINT p_count)
{
	static char ansiTip[128];
	std::vector<ShellServices::eNotificationIconInfo> request(p_count);
	std::vector<PVOID> trayInfo(p_count);
	for (UINT i = 0; i< p_count; ++i)
	{
		request[i] = g_trayInfoMapping[p_info[i]];
	}
	if (!m_imp->GetNotificationIconInfo(p_icon,&(trayInfo[0]),&(request[0]),p_count))
	{
		return false;
	}
	for (UINT i = 0; i< p_count; ++i)
	{
		switch (request[i])
		{
		case ShellServices::NI_LEGACY:
		case ShellServices::NI_VERSION:
			p_return[i] = trayInfo[i];
			break;
		case ShellServices::NI_TIP:
			if (p_info[i] == FindAtom(TEXT("Trayicon::AnsiTip")))
			{
				WideCharToMultiByte(CP_ACP,0,reinterpret_cast<LPCWSTR>(trayInfo[i]),-1,ansiTip,128,NULL,NULL);
				p_return[i] = ansiTip;
			}
			else
			{
				p_return[i] = trayInfo[i];
			}
			break;
		default:
			return false;
		}
	}
	return true;
}

bool NotifyIconSrv::Call(ATOM p_function, const ServiceArg &p_arg1)
{
	if (p_function == m_GetTraySizeFn)
	{
		typedef Arg<UINT &> type1;
		if (CHECK_ARG(1))
		{
			MAKE_ARG(1) = m_imp->GetTraySize();
			return true;
		}
		PRINT("Argument mismatch in " SERVICE_NAME ", 1 Arg call");
	}
	PRINT("1 Arg call not found in " SERVICE_NAME);
	return false;
}

bool NotifyIconSrv::Call(ATOM p_function, const ServiceArg &p_arg1, const ServiceArg &p_arg2)
{
	if (p_function == m_lookupIconFn)
	{
		typedef Arg<NotificationIcon *&> type1;
		typedef Arg<UINT> type2;
		if (CHECK_ARG(1) && CHECK_ARG(2))
		{
			MAKE_ARG(1) = m_imp->LookupIcon(MAKE_ARG(2));
			return true;
		}
		PRINT("Argument mismatch in " SERVICE_NAME ", 2 Arg call");
	}
	PRINT("2 Arg call not found in " SERVICE_NAME);
	return false;
}

bool NotifyIconSrv::Call(ATOM p_function, const ServiceArg &p_arg1,
						 const ServiceArg &p_arg2, const ServiceArg &p_arg3)
{
	if (p_function == m_lookupIconFn)
	{
		typedef Arg<NotificationIcon *&> type1;
		typedef Arg<HWND> type2;
		typedef Arg<UINT> type3;
		if (CHECK_ARG(1) && CHECK_ARG(2) && CHECK_ARG(3))
		{
			MAKE_ARG(1) = m_imp->LookupIcon(MAKE_ARG(2), MAKE_ARG(3));
			return true;
		}
		PRINT("Argument mismatch in " SERVICE_NAME ", 3 Arg call");
	}
	PRINT("3 Arg call not found in " SERVICE_NAME);
	return false;
}

bool NotifyIconSrv::Call(ATOM p_function, const ServiceArg &p_arg1,
						 const ServiceArg &p_arg2, const ServiceArg &p_arg3,
						 const ServiceArg &p_arg4)
{
	if (p_function == m_getNotificationIconInfoFn)
	{
		typedef Arg<NotificationIcon *> type1;
		typedef Arg<PVOID *> type2;
		typedef Arg<ATOM *> type3;
		typedef Arg<UINT> type4;
		if (CHECK_ARG(1) && CHECK_ARG(2) && CHECK_ARG(3) && CHECK_ARG(4))
		{
			return GetNotificationIconInfo(MAKE_ARG(1), MAKE_ARG(2), MAKE_ARG(3), MAKE_ARG(4));
		}
		PRINT("Argument mismatch in " SERVICE_NAME ", 4 Arg call");
	}
	PRINT("4 arg not found  in " SERVICE_NAME);
	return false;
}

REGISTER_SERVICE(NotifyIconSrv)

}
