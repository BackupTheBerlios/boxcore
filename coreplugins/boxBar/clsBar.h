#ifndef CLSBAR_H
#define CLSBAR_H

#include "clsItemCollection.h"
#include <tchar.h>

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
	virtual void configMenu(Menu *pMenu);
protected:
	void populateBar();
private:
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
	int sizePercentage;
};

#endif // CLSBAR_H
