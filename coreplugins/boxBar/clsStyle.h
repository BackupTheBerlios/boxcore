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
	int getStyleTextJustify(int pStyle);
protected:
	StyleItem *toolbar;
	StyleItem *toolbarClock;
	StyleItem *toolbarLabel;
	StyleItem *toolbarWindowLabel;
	StyleItem *toolbarButton;
	StyleItem *toolbarButtonPressed;
	HFONT toolbarFont;
private:
};

#endif // CLSSTYLE_H
