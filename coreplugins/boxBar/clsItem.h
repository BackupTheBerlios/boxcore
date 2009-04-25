#ifndef CLSITEM_H
#define CLSITEM_H
/** @file clsItem.h
  * @brief Contains definition for clsItem and supporting structures
  */

#include "BBApi.h"
#include <windows.h>

#include "clsStyle.h"
#include "../utils/clsApiLoader.h"
#include "../../dynwinapi/clsMsimg32.h"
#include "../../debug/debug.h"
#include "../../utility/stringcopy.h"
#include "limits.h"

#include <vector>
#include "clsSettingsManager.h"

/** @enum dimType
  * @brief Dimensions for use in boxBar
  *
  * Implements flags for vertical and horizontal, as well flags for
  * none and both. The both flag is constructed to allow bit testing with the
  * other two direction flags to work.
  */
enum dimType {DIM_NONE = 0, DIM_HORIZONTAL = 1, DIM_VERTICAL = 2, DIM_BOTH = 3};

//Message for internal communications

#define BOXBAR_REMOVEITEM WM_USER + 1
#define BOXBAR_UPDATESIZE WM_USER + 2
#define BOXBAR_NEEDTIP WM_USER + 3
#define BOXBAR_REDRAW WM_USER + 4
#define BOXBAR_QUEUEBALLOON WM_USER + 5
#define BOXBAR_CLEANBALLOONS WM_USER + 6

#define BOXBAR_BALLOONDONE WM_USER+100

/** @internal
 * @brief Contains all classes, functions, etc. for the @ref boxBar "boxBar" plugin
 */
namespace boxBar
{

class Item;

/** @typedef mouseFunction
  * @brief Mouseclick callback signature
  */
typedef void (*mouseFunction)(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam);

static int dummyMaxInt = INT_MAX;
static int dummyZeroInt = 0;

struct minMaxStruct
{
	minMaxStruct(INT &p_minX = dummyZeroInt, INT &p_maxX = dummyMaxInt, INT &p_minY = dummyZeroInt, INT &p_maxY = dummyMaxInt):m_minX(p_minX), m_maxX(p_maxX), m_minY(p_minY), m_maxY(p_maxY) {}
	INT &m_minX;
	INT &m_maxX;
	INT &m_minY;
	INT &m_maxY;
};

/** @internal
  * @brief Base class for boxBar
  *
  * clsItem implements common functionality for all parts of boxBar. The functions
  * implemented include mouse-click detection with handlers, a basic drawing function,
  * basic resize and move support as well as tooltip support.
  */
class Item
{
public:
	Item(LPCSTR p_itemName = NULL, minMaxStruct p_minMax = minMaxStruct());
	virtual ~Item();
	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void draw(HDC pContext) = 0;
	void drawNow();
	virtual dimType resize(int pX, int pY);
	virtual void move(int pX, int pY);
	dimType getKnowsSize()
	{
		return m_knowsSize;
	}
	dimType getWantsStretch()
	{
		return m_wantsStretch;
	}
	virtual void readSettings();

	virtual void calculateSizes(bool pSizeGiven = false);

	virtual void configMenu(Menu *pMenu, bool p_update = false);

	virtual bool AssignButton(LPCSTR p_data, mouseFunction &p_hook, LPCSTR &p_broamSlot);

	int getSize(dimType pDim);
	bool hitTest(int pX, int pY);
protected:
	/** @brief Directions in which the item can calculate its size
	  *
	  * Set to DIM_BOTH if the item can calculate complete dimensions
	  */
	dimType m_knowsSize;

	/** @brief Directions in which the item wants to be stretched if possible*/
	dimType m_wantsStretch;

	/** @brief The area occupied by the item
	  *
	  * Used when testing for clicks and drawing.
	  * @note Coordinates are in pixels relative to the client area of the containing window
	  */
	RECT itemArea;

	int minSizeX;
	int minSizeY;

	INT m_minSizeX;
	INT m_minSizeY;
	INT &m_maxSizeX;
	INT m_maxSizeY;

	/** @brief The styleitem to be used for drawing
	  *
	  * If set to 0 no drawing will be performed in this item by default. When
	  * set to a value other than 0, this causes a gradient to be drawn on the item
	  * based on the current style.
	  */
	UINT style;
	/// @brief Controls whether to use the alpha-blending drawing routines or the standard drawing routines
	static bool alphaDraw;

	/// @brief Alpha
	BYTE itemAlpha;

	bool mouseDown;
	bool m_dblClk;
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

	mouseFunction m_leftDblClick;

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
	static LPCSTR configFile;
	static clsStyle bbStyle;

	static clsApiLoader bbApiLoader;
	static HWND hBlackboxWnd;
	static HINSTANCE hInstance;

	static LPCSTR m_pluginPrefix;
	LPSTR m_itemPrefix;

	LPCSTR PluginBroam(LPSTR p_buffer, LPCSTR p_broam);
	LPCSTR ItemBroam(LPSTR p_buffer, LPCSTR p_broam);

	LPCSTR ItemRCKey(LPSTR p_buffer, LPCSTR p_key);

	static int getTimerID()
	{
		static int id = 10;
		return id++;
	}
	void initTooltips();
	void setTooltip();
	void ClearTooltip();
	TCHAR *tipText;
	bool m_hasTooltip;

	static void broam(Item *p_item, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam);

	const char* m_broamLeft;
	const char* m_broamLeftDbl;
	const char* m_broamRight;
	const char* m_broamMid;

	HRGN m_clipRegion;

	static SettingsManager s_settingsManager;

	BLENDFUNCTION itemBlend;
	BITMAPINFO itemBitmapInfo;
private:
};

}

#endif // CLSITEM_H
