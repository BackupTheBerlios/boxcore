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

class ItButton: public clsItemCollection
{
public:
	ItButton();
	virtual ~ItButton();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
protected:
	clsTextItem *m_buttonText;
};

#endif /* CLSITBUTTON_H_ */
