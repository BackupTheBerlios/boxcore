/*
 * clsItButton.h
 *
 *  Created on: 16 Jan 2009
 *      Author: shaun
 */

#ifndef CLSITBUTTON_H_
#define CLSITBUTTON_H_

#include "clsCollection.h"

namespace boxBar
{

class Text;
class Icon;

class Button: public Collection
{
public:
	Button(LPCSTR p_itemName = "Button");
	virtual ~Button();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
protected:
	bool &m_hasIcon;
	bool &m_hasText;
	tstring &m_buttonText;

	Icon *m_iconItem;
	Text *m_textItem;
};

}

#endif /* CLSITBUTTON_H_ */
