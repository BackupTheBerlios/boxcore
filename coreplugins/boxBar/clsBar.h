#ifndef CLSBAR_H
#define CLSBAR_H

#include "clsItemCollection.h"
#include <tchar.h>

typedef void(*fnSetTaskbarPos)(int pLeft, int pTop, int pRight, int pBottom, UINT pEdge);
enum ePosition {POS_TOP = 1, POS_VCENTER = 2, POS_BOTTOM = 4, POS_LEFT = 8, POS_CENTER = 16, POS_RIGHT = 32};


class clsBar : public clsItemCollection
{
	public:
		clsBar(TCHAR *pClassName, HINSTANCE pInstance, bool pVertical = false);
		virtual ~clsBar();
		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK realWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual void calculateSizes(bool pSizeGiven = true);

		virtual dimType resize(int pX, int pY);
	protected:
		void populateBar();
		void configMenu();
	private:
		TCHAR className[100];
		bool moving;
		bool trackMouse;
		bool setMargin;

		int margin;
		int marginEdge;

		UINT barLocation;
		int sizePercentage;

		fnSetTaskbarPos SetTaskbarPos;
};

#endif // CLSBAR_H
