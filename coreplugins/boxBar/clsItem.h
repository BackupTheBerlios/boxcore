#ifndef CLSITEM_H
#define CLSITEM_H
/** @file clsItem.h
  * @brief Contains definition for clsItem and supporting structures
  */

#define WINVER 0x502
#include <windows.h>

#include "clsStyle.h"
#include "../utils/clsApiLoader.h"

/** @enum dimType
  * @brief Dimentions for use in boxBar
  *
  * Implements flags for vertical and horizontal, as well flags for
  * none and both. The both flag is constructed to allow bit testing with the
  * other two direction flags to work.
  */
enum dimType {DIM_NONE = 0, DIM_HORIZONTAL = 1, DIM_VERTICAL = 2, DIM_BOTH = 3};

//Message for internal communications

#define BOXBAR_REMOVEITEM WM_USER + 1
#define BOXBAR_UPDATESIZE WM_USER + 2

class clsItem;

/** @typedef mouseFunction
  * @brief Mousclick callback signature
  */
typedef void (*mouseFunction)(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);

/** @class clsItem
  * @brief Base class for boxBar
  *
  * clsItem implements common functionality for all parts of boxBar. The functions
  * implemented include mouse-click detection with handlers, a basic drawing function,
  * basic resize and move support as well as tooltip support.
  */
class clsItem
{
public:
	clsItem(bool pVertical);
	virtual ~clsItem();
	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void draw(HDC pContext);
	virtual dimType resize(int pX, int pY);
	virtual void move(int pX, int pY);
	virtual dimType getFixed() {
		return fixed;
	}
	virtual void readSettings();

	virtual void calculateSizes(bool pSizeGiven = false);

	virtual void configMenu(Menu *pMenu);

	int getSize(dimType pDim);
	bool hitTest(int pX, int pY);
protected:
	/** @brief Vertical state of the item
	  *
	  * May affect drawing, sizing or calculating functions
	  */
	bool vertical;

	/** @brief Directions in which the item can caluclate its size
		  *
		  * Set to DIM_BOTH if the item can calculate complete dimensions
		  */
	dimType fixed;

	/** @brief The area occupied by the item
	  *
	  * Used when testing for clicks and drawing.
	  * @note Coordinates are in pixels relative to the client area of the containing window
	  */
	RECT itemArea;

	int minSizeX;
	int minSizeY;

	/** @brief The styleitem to be used for drawing
	  *
	  * If set to 0 no drawing will be performed in this item by default. When
	  * set to a value other than 0, this causes a gradient to be drawn on the item
	  * based on the current style.
	  */
	UINT style;


	bool mouseDown;
	int mouseButton;


	/** @brief Right-click mouse hook
	  *
	  * This member should be set if the item wants to respond to normal right-clicks
	  */
	mouseFunction rightClick;

	/** @brief Left-click mouse hook
	  *
	  * This member should be set if the item wants to respond to normal Left-clicks
	  */
	mouseFunction leftClick;

	/** @brief Mid-click mouse hook
	  *
	  * This member should be set if the item wants to respond to normal mid-clicks
	  */
	mouseFunction midClick;
	/** @brief X1-click mouse hook
	  *
	  * This member should be set if the item wants to respond to normal X1-clicks
	  */
	mouseFunction X1Click;

	/** @brief X2-click mouse hook
	  *
	  * This member should be set if the item wants to respond to normal X2-clicks
	  */
	mouseFunction X2Click;

	/** @brief Tooltip window handle
	  *
	  * Initialized on the first call to setTooltip. Used for all subsequent tooltip operations
	  */
	static HWND tooltipWnd;


	static HWND barWnd;
	static CHAR configFile[MAX_PATH];
	static clsStyle bbStyle;

	static clsApiLoader bbApiLoader;
	static HWND hBlackboxWnd;
	static HINSTANCE hInstance;

	static int getTimerID() {
		static int id = 10;
		return id++;
	}
	void initTooltips();
	void setTooltip();
	TCHAR *tipText;
private:
};

#endif // CLSITEM_H
