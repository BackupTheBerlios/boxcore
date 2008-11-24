#include "clsStyle.h"

clsStyle::clsStyle()
{
	toolbar = (StyleItem *)GetSettingPtr(SN_TOOLBAR);
	toolbarClock = (StyleItem *)GetSettingPtr(SN_TOOLBARCLOCK);
	toolbarClockFont = CreateStyleFont(toolbar);
}

clsStyle::~clsStyle()
{
	//dtor
}

/** @brief getStyleFont
  *
  * @todo: document this function
  */
HFONT clsStyle::getStyleFont(int pStyle)
{
	switch (pStyle)
	{
		case SN_TOOLBARCLOCK:
			return toolbarClockFont;
	}
}

/** @brief getStyleBorder
  *
  * @todo: document this function
  */
bool clsStyle::getStyleBorder(int pStyle)
{
	switch (pStyle)
	{
		case SN_TOOLBAR:
			return toolbar->bordered;
		case SN_TOOLBARCLOCK:
			return toolbarClock->bordered;
	}
}

/** @brief getStyleBorder
  *
  * @todo: document this function
  */
COLORREF clsStyle::getStyleTextColor(int pStyle)
{
	switch (pStyle)
	{
		case SN_TOOLBAR:
			return toolbar->TextColor;
		case SN_TOOLBARCLOCK:
			return toolbarClock->TextColor;
	}
}

/** @brief getStyle
  *
  * @todo: document this function
  */
StyleItem * clsStyle::getStyle(int pStyle)
{
	switch(pStyle)
	{
		case SN_TOOLBAR:
			return toolbar;
		case SN_TOOLBARCLOCK:
			return toolbarClock;
	}
}

