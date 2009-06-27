/*
 * clsShellServiceObjectsSrv.cpp
 *
 *  Created on: 04 May 2009
 *      Author: shaun
 */

#include "clsShellServiceObjectsSrv.h"
#include "clsServiceRegistrar.h"

#define SERVICE_NAME "SRV_SSO"

namespace ShellServices
{

struct SSO_MSG_V1
{
	CLSID clsid;
};

struct SSO_MSG_V2
{
	CLSID clsid;
	DWORD dwCommand;
};

ShellServiceObjectsSrv::ShellServiceObjectsSrv():
		Service(SERVICE_NAME)
{
	// TODO Auto-generated constructor stub

}

ShellServiceObjectsSrv::~ShellServiceObjectsSrv()
{
	// TODO Auto-generated destructor stub
}

HRESULT ShellServiceObjectsSrv::ProcessMessage(DWORD p_cbData, PVOID p_lpData)
{
	switch (p_cbData)
	{
	case sizeof(SSO_MSG_V1):
	{
		PRINT("SSO control message received (SHLoadInProc Version)");
		if (s_systemInfo.isOsVista())
		{
			PRINT("SHLoadInProc not implemented under vista, to match explorer");
			return E_NOTIMPL;
		}
		else
		{
			SSO_MSG_V1 &msg = *reinterpret_cast<SSO_MSG_V1 *>(p_lpData);
			StartGUID(msg.clsid);
			return TRUE;
		}
	}
	case sizeof(SSO_MSG_V2):
	{
		PRINT("SSO control message received (SHEnableServiceObject Version)");
		SSO_MSG_V2 &msg = *reinterpret_cast<SSO_MSG_V2 *>(p_lpData);
		switch (msg.dwCommand)
		{
		case 1:
			PRINT("SHLoadInProc message");
			if (s_systemInfo.isOsVista())
			{
				PRINT("SHLoadInProc not implemented under vista, to match explorer");
				return E_NOTIMPL;
			}
			else
			{
				StartGUID(msg.clsid);
				return TRUE;
			}
		case 2:
			PRINT("SHEnableServiceObject with FALSE");
			StopGUID(msg.clsid);
			return TRUE;
		case 3:
			PRINT("SHEnableServiceObject with TRUE");
			StartGUID(msg.clsid);
			return TRUE;
		default:
			PRINT("Unknown command type");
			return E_NOTIMPL;
		}
	}
	default:
		PRINT("SSO control message received, but we don't understand it");
		return E_NOTIMPL;
	}
	return 0;
}

bool ShellServiceObjectsSrv::_Start()
{
	if (!m_running)
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE | COINIT_SPEED_OVER_MEMORY);
		if (s_systemInfo.isOsVista())
		{
			StartGUID(s_CLSID_ShellTrayObj);
		}
		else
		{
			StartRegistryValues(TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ShellServiceObjectDelayLoad"));
		}
		s_serviceManager->Call("SRV_ShellTrayWnd",
							   "FN_SetHandler",
							   Arg<UINT>(2),
							   Arg<ShellServiceHandler *>(this));
		m_running = true;
	}
	else
	{
		PRINT("Repeated start attempt");
	}
	return m_running;
}

bool ShellServiceObjectsSrv::_Stop()
{
	s_serviceManager->Call("SRV_ShellTrayWnd", "FN_SetHandler", Arg<UINT>(2), Arg<ShellServiceHandler *>(NULL));
	for (SSOlist_t::iterator i = m_SSOlist.begin(); i != m_SSOlist.end(); ++i)
	{
		i->second->Exec(&CGID_ShellServiceObject, OLECMDID_SAVE, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
		i->second->Release();
	}
	m_SSOlist.clear();
	CoUninitialize();
	m_running = false;
	return true;
}

void ShellServiceObjectsSrv::StartRegistryValues(LPCTSTR p_regKey)
{
	HKEY regKey;
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, p_regKey, 0, KEY_READ, &regKey))
	{
		TRACE("Could not open registry key");
		return;
	}
	WCHAR valueName[MAX_PATH];
	DWORD valueNameSize;
	WCHAR valueData[40];
	DWORD valueDataSize;
	for (DWORD i = 0;;++i)
	{
		valueNameSize = sizeof(valueName)/sizeof(TCHAR);
		valueDataSize = sizeof(valueData);
		if (ERROR_SUCCESS != RegEnumValueW(regKey, i, valueName, &valueNameSize, NULL, NULL, (BYTE *)valueData, &valueDataSize))
		{
			RegCloseKey(regKey);
			return;
		}
		CLSID clsid;
		CLSIDFromString(valueData, &clsid);
		StartGUID(clsid);
	}
}

void ShellServiceObjectsSrv::StartGUID(CLSID p_id)
{
	if (!s_systemInfo.isOsVista() || m_SSOlist.size() == 0)
	{
		IOleCommandTarget *pOCT;
		HRESULT hr = CoCreateInstance(p_id, NULL, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, IID_IOleCommandTarget, (void **) &pOCT);
		if (SUCCEEDED(hr))
		{
			pOCT->Exec(&CGID_ShellServiceObject, OLECMDID_NEW, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
			m_SSOlist.push_back(std::pair<GUID, IOleCommandTarget *>(p_id, pOCT));
		}
	}
	else
	{
		m_SSOlist.front().second->Exec(&p_id, OLECMDID_NEW, 3, NULL, NULL);
	}
}

void ShellServiceObjectsSrv::StopGUID(CLSID p_id)
{
	if (s_systemInfo.isOsVista())
	{
		m_SSOlist.front().second->Exec(&p_id, OLECMDID_NEW, 2, NULL, NULL);
	}
	else
	{
		for (SSOlist_t::iterator i = m_SSOlist.begin(); i != m_SSOlist.end(); ++i)
		{
			if (i->first == p_id)
			{
				i->second->Exec(&CGID_ShellServiceObject, OLECMDID_SAVE, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
				i->second->Release();
				m_SSOlist.erase(i);
				return;
			}
		}
	}
}

GUID ShellServiceObjectsSrv::s_CLSID_ShellTrayObj = { 0x35CEC8A3, 0x2BE6, 0x11D2,
		{ 0x87, 0x73, 0x92, 0xE2, 0x20, 0x52, 0x41, 0x53 }
													};

REGISTER_SERVICE(ShellServiceObjectsSrv)

}
