#ifndef CLSBAR_H
#define CLSBAR_H

#include "clsItemCollection.h"
#include <tchar.h>
#include <deque>

class Tip;

namespace boxBar
{

using std::deque;

enum ePosition {POS_TOP = 1, POS_VCENTER = 2, POS_BOTTOM = 4, POS_LEFT = 8, POS_CENTER = 16, POS_RIGHT = 32};


class clsBar : public clsItemCollection
{
public:
	clsBar(TCHAR *pClassName, HINSTANCE pInstance, HWND pSlit, bool pVertical = false);
	virtual ~clsBar();
	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK realWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void calculateSizes(bool pSizeGiven = true);

	virtual dimType resize(int pX, int pY);
	virtual void readSettings();
	virtual void configMenu(Menu *pMenu, bool p_update = false);

	void QueueTip(Tip *p_tip);
	void KillTips(HWND p_hWnd, UINT p_uID);
protected:
	void populateBar();
private:
	void ShowTip(Tip *p_tip);

	HDC buffer;
	HBITMAP bufferBitmap;
	HBITMAP origBitmap;
	BITMAPINFO bufferInfo;

	HBITMAP brushBitmap;
	HBRUSH eraseBrush;

	TCHAR className[100];
	bool moving;
	bool trackMouse;
	bool setMargin;

	bool toggleWithPlugins;

	bool useSlit;
	HWND slitWnd;
	bool inSlit;

	bool enableTransparency;
	bool userPerPixel;
	BLENDFUNCTION barBlend;

	bool hasTray;

	int margin;
	int marginEdge;

	UINT barLocation;
	UINT m_barHGrowth;
	UINT m_barVGrowth;
	int sizePercentage;

	deque<Tip *> m_tipQueue;
	Tip * m_activeTip;
	UINT m_tipTimer;
	bool m_replaceTip;
};

}

#endif // CLSBAR_H
