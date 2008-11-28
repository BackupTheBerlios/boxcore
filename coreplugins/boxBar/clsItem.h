#ifndef CLSITEM_H
#define CLSITEM_H

#define WINVER 0x502
#include <windows.h>

#include "clsStyle.h"
#include "../utils/clsApiLoader.h"

enum dimType {DIM_NONE = 0, DIM_HORIZONTAL = 1, DIM_VERTICAL = 2, DIM_BOTH = 3};

//Message for internal communications

#define BOXBAR_REMOVEITEM WM_USER + 1
#define BOXBAR_UPDATESIZE WM_USER + 2

class clsItem;

typedef void (*mouseFunction)(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);

class clsItem
{
	public:
		clsItem(bool pVertical);
		virtual ~clsItem();
		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual bool hitTest(int pX, int pY);
		virtual void draw(HDC pContext);
		virtual dimType resize(int pX, int pY);
		virtual void move(int pX, int pY);
		virtual dimType getFixed() {return fixed;}
		virtual int getSize(dimType pDim);
		virtual void calculateSizes(bool pSizeGiven = false);
	protected:
		bool vertical;
		dimType fixed;
		RECT itemArea;
		int minSizeX;
		int minSizeY;

		UINT style;

		bool mouseDown;
		int mouseButton;

		mouseFunction rightClick;
		mouseFunction leftClick;
		mouseFunction midClick;
		mouseFunction X1Click;
		mouseFunction X2Click;

		static HWND tooltipWnd;

		static HWND barWnd;
		static CHAR configFile[MAX_PATH];
		static clsStyle bbStyle;

		static clsApiLoader bbApiLoader;
		static HWND hBlackboxWnd;
		static HINSTANCE hInstance;

		static int getTimerID() {static int id=10; return id++;}
		void initTooltips();
		void setTooltip(TCHAR *pText);
		TCHAR *tipText;
	private:
};

#endif // CLSITEM_H
