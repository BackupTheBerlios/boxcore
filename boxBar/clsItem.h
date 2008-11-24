#ifndef CLSITEM_H
#define CLSITEM_H

#include <windows.h>

#include "clsStyle.h"
#include "../utils/clsApiLoader.h"

enum dimType {DIM_NONE = 0, DIM_HORIZONTAL = 1, DIM_VERTICAL = 2, DIM_BOTH = 3};

//Message for internal communications

#define BOXBAR_REMOVEITEM WM_USER + 1
#define BOXBAR_UPDATESIZE WM_USER + 2


class clsItem
{
	public:
		clsItem(bool pVertical);
		virtual ~clsItem();
		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
		virtual bool hitTest(int pX, int pY);
		virtual void draw(HDC pContext) = 0;
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

		bool mouseDown;
		int mouseButton;

		static HWND barWnd;
		static CHAR configFile[MAX_PATH];
		static clsStyle bbStyle;

		static clsApiLoader bbApiLoader;

		static int getTimerID() {static int id=10; return id++;}

	private:
};

#endif // CLSITEM_H
