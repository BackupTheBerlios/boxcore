/*
 * clsTip.h
 *
 *  Created on: 05 Jan 2009
 *      Author: shaun
 */

#ifndef CLSTIP_H_
#define CLSTIP_H_

#include <windows.h>
#include "clsStyle.h"

class Tip
{
public:
	Tip(HINSTANCE p_instance, HWND p_hWnd, UINT p_uID, UINT p_uCallbackMessage, UINT p_uVersion, LPCSTR p_info, LPCSTR p_infoTitle, UINT p_timeout);
	virtual ~Tip();

	void Position(INT p_x, INT p_y);

	bool BelongsTo(HWND p_hWnd, UINT p_uID);

	void Display();
	void Timeout();
	void Click();
	void Kill();
	static LRESULT CALLBACK realWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	LPSTR m_info;
	LPSTR m_infoTitle;
	UINT m_timeout;

	HWND m_tipWindow;

	HWND m_iconWnd;
	UINT m_iconID;
	UINT m_iconCallback;
	UINT m_iconVersion;

	HINSTANCE m_instance;

	bool m_leftDown;
	bool m_rightDown;

	static clsStyle bbStyle;
};

#endif /* CLSTIP_H_ */
