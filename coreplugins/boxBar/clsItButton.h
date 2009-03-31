/*
 * clsItButton.h
 *
 *  Created on: 16 Jan 2009
 *      Author: shaun
 */

#ifndef CLSITBUTTON_H_
#define CLSITBUTTON_H_

#include "clsItemCollection.h"

class clsTextItem;
class clsIconItem;

class ItButton: public clsItemCollection
{
public:
	ItButton(LPCSTR p_itemName = "Button");
	virtual ~ItButton();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
protected:
	bool &m_hasIcon;
	bool &m_hasText;
	std::string &m_buttonText;

	clsIconItem *m_iconItem;
	clsTextItem *m_textItem;
};

#endif /* CLSITBUTTON_H_ */
