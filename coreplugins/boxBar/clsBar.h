#ifndef CLSBAR_H
#define CLSBAR_H

#include "clsCollection.h"
#include "../utils/clsPlugin.h"
#include <tchar.h>
#include <deque>

class Tip;

namespace Plugin_boxBar
{

using std::deque;

enum ePosition {POS_TOP = 1, POS_VCENTER = 2, POS_BOTTOM = 4, POS_LEFT = 8, POS_CENTER = 16, POS_RIGHT = 32};


class Bar : public Plugin, public Collection
{
public:
	Bar(TCHAR *pClassName, HINSTANCE pInstance, HWND pSlit, bool pVertical = false);
	virtual ~Bar();
	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK realWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void calculateSizes(bool pSizeGiven = true);

	virtual dimType resize(int pX, int pY);
	virtual void readSettings();
	virtual void configMenu(Menu *pMenu, bool p_update = false);

	void QueueTip(Tip *p_tip);
	void KillTips(HWND p_hWnd, UINT p_uID);

	static void SetPluginName(TCHAR *p_pluginName);
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
	INT &sizePercentage;

	deque<Tip *> m_tipQueue;
	Tip * m_activeTip;
	UINT m_tipTimer;
	bool m_replaceTip;

	INT BeginPlugin();
		void EndPlugin();
};

}

#endif // CLSBAR_H
