#ifndef CLSBOXBAR_H
#define CLSBOXBAR_H

#include "clsBar.h"
#include <tchar.h>


class clsBoxBar
{
public:
	clsBoxBar();
	virtual ~clsBoxBar();

	int beginPluginEx(HINSTANCE hPluginInstance, HWND hSlit);
	void endPlugin(HINSTANCE hPluginInstance);
protected:
private:
	TCHAR pluginName[100];
	HINSTANCE hPlugin;

	HWND hBlackboxWnd;
	HWND hSlitWnd;

	bool started;

	clsBar *bar;
};

#endif // CLSBOXBAR_H
