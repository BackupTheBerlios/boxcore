/*
 * clsPlugin.h
 *
 *  Created on: 11 Apr 2009
 *      Author: shaun
 */

#ifndef CLSPLUGIN_H_
#define CLSPLUGIN_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

class Plugin
{
public:
	Plugin(HINSTANCE p_hInstance);
	virtual ~Plugin();

	INT BeginPlugin(HINSTANCE p_hInstance, HWND p_hSlit = NULL);
	void EndPlugin(HINSTANCE p_hInstance);
protected:
	virtual INT BeginPlugin() = 0;
	virtual void EndPlugin() = 0;

	HINSTANCE m_hInstance;
	HWND m_hSlit;
	std::string m_pluginName;
	std::string m_rcPath;
};

#endif /* CLSPLUGIN_H_ */
