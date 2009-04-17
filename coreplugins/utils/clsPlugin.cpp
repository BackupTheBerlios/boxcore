/*
 * clsPlugin.cpp
 *
 *  Created on: 11 Apr 2009
 *      Author: shaun
 */

#include "clsPlugin.h"
#include "BBApi.h"

Plugin::Plugin(HINSTANCE p_hInstance):m_hInstance(p_hInstance)
{
	CHAR pluginPath[MAX_PATH];
	GetModuleFileNameA(m_hInstance, pluginPath, MAX_PATH);
	CHAR *pluginName = strrchr(pluginPath, '\\') + 1;
	m_pluginName = pluginName;
	m_pluginName.resize(m_pluginName.find_last_of('.'));
	*(strrchr(pluginPath, '\\')+1) = '\0';
	std::string rcFile(m_pluginName);
	rcFile += ".rc";
	LPCSTR configFile = ConfigFileExists(rcFile.c_str(), pluginPath);
	if (strlen(configFile))
	{
	m_rcPath = configFile;
	}
	else
	{
		m_rcPath = pluginPath;
		m_rcPath += rcFile.c_str();
	}
}

Plugin::~Plugin()
{
	// TODO Auto-generated destructor stub
}

INT Plugin::BeginPlugin(HINSTANCE p_hInstance, HWND p_hSlit)
{
	if (p_hInstance == m_hInstance)
	{
		if (!FindAtomA(m_pluginName.c_str()))
		{
			AddAtomA(m_pluginName.c_str());
			m_hSlit = p_hSlit;
			return BeginPlugin();
		}
		else
		{
			CHAR msg[MAX_PATH];
			printf(msg,"A plugin called %s is already loaded. Please rename this plugin DLL before loading to avoid conflicts in broadcast messages", m_pluginName.c_str());
			MessageBoxA(NULL, msg, m_pluginName.c_str(), MB_OK | MB_TOPMOST);
			return 1;
		}
	}
	else
	{
		return 1;
	}
}

void Plugin::EndPlugin(HINSTANCE p_hInstance)
{
	if (p_hInstance == m_hInstance)
	{
		DeleteAtom(FindAtomA(m_pluginName.c_str()));
		EndPlugin();
	}
}




