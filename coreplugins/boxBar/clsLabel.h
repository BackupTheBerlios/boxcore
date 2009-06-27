#ifndef CLSLABELITEM_H
#define CLSLABELITEM_H

#include "clsCollection.h"
#include "clsText.h"

namespace boxBar
{

class Label : public Collection
{
public:
	Label(bool pVertical, LPCSTR p_itemName = "Label");
	virtual ~Label() {};

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void SetStyle(UINT p_style);
protected:
	Text *m_textItem;
};

}

#endif // CLSLABELITEM_H
