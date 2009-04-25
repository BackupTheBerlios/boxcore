#ifndef CLSFLEXISPACER_H
#define CLSFLEXISPACER_H

#include "clsItem.h"

namespace boxBar
{

class clsFlexiSpacer : public Item
{
public:
	clsFlexiSpacer();
	virtual ~clsFlexiSpacer();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
	virtual void draw(HDC pContext) {}
protected:
private:
};

}

#endif // CLSFLEXISPACER_H
