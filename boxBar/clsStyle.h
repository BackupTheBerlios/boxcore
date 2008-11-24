#ifndef CLSSTYLE_H
#define CLSSTYLE_H

#include "BBApi.h"

class clsStyle
{
	public:
		clsStyle();
		virtual ~clsStyle();

		StyleItem *getStyle(int pStyle);
		bool getStyleBorder(int pStyle);
		HFONT getStyleFont(int pStyle);
		COLORREF getStyleTextColor(int pStyle);
	protected:
		StyleItem *toolbar;
		StyleItem *toolbarClock;
		StyleItem *toolbarLabel;
		StyleItem *toolbarButton;
		StyleItem *toolbarButtonPressed;
		HFONT toolbarClockFont;
	private:
};

#endif // CLSSTYLE_H
