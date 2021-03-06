/*
 ============================================================================

  This file is part of the bbLean source code
  Copyright ｩ 2001-2003 The Blackbox for Windows Development Team
  Copyright ｩ 2004 grischka

  http://bb4win.sourceforge.net/bblean
  http://sourceforge.net/projects/bb4win

 ============================================================================

  bbLean and bb4win are free software, released under the GNU General
  Public License (GPL version 2 or later), with an extension that allows
  linking of proprietary modules under a controlled interface. This means
  that plugins etc. are allowed to be released under any license the author
  wishes. For details see:

  http://www.fsf.org/licenses/gpl.html
  http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.

 ============================================================================
*/

#include "../BB.h"
#include "../Settings.h"
#include "MenuMaker.h"
#include "Menu.h"

int FolderItem::m_nBulletPosition = DT_RIGHT;
int FolderItem::m_nBulletStyle = BS_TRIANGLE;
int FolderItem::m_nScrollPosition = DT_LEFT;

//===========================================================================

FolderItem::FolderItem(Menu* pSubMenu, const char* pszTitle) : MenuItem(pszTitle)
{
	m_nSortPriority = M_SORT_FOLDER;
	m_ItemID = MENUITEM_ID_FOLDER;
	LinkSubmenu(pSubMenu);
}

void FolderItem::Invoke(int button)
{
	if (INVOKE_LEFT == button)
	{
		if (m_pszCommand)
		{
			m_pMenu->hide_on_click();
			post_command(m_pszCommand);
			return;
		}

		if (m_pSubMenu)
			m_pSubMenu->SetPinned(false);

		ShowSubMenu();
	}
}

//===========================================================================
// Paints the folder item, the parent class will paint first

void FolderItem::Paint(HDC hDC)
{
	MenuItem::Paint(hDC);

	COLORREF C =
		m_bActive
		? mStyle.MenuHilite.foregroundColor
		: (MI_NOP_DISABLED & m_isNOP)
		? mStyle.MenuFrame.disabledColor
		: mStyle.MenuFrame.foregroundColor
		;

	int r = mStyle.bulletUnix ? 2 : 3;

	int yOffset = m_nTop + m_nHeight / 2;
	int xOffset;
	int is = m_pMenu->m_iconSize;
	if (-2 == is) is = MenuInfo.nIconSize;
	if (FolderItem::m_nBulletPosition == DT_RIGHT)
		xOffset = m_nLeft + m_nWidth - MenuInfo.nItemIndent[is] / 2 - 1;
	else
		xOffset = m_nLeft + MenuInfo.nItemIndent[is] / 2;

	HGDIOBJ oldPen = SelectObject(hDC, CreatePen(PS_SOLID, 1, C));
	switch (m_nBulletStyle)
	{
	case BS_TRIANGLE:
		arrow_bullet (hDC, xOffset, yOffset, m_nBulletPosition == DT_LEFT ? -1 : 1);
		break;

	case BS_SQUARE:
		MoveToEx(hDC, xOffset + r, yOffset + r, NULL);
		LineTo  (hDC, xOffset + r, yOffset - r);
		LineTo  (hDC, xOffset - r, yOffset - r);
		LineTo  (hDC, xOffset - r, yOffset + r);
		LineTo  (hDC, xOffset + r, yOffset + r);
		break;

	case BS_DIAMOND:
		int  x1, x2, y;
		for (x2=1+(x1=xOffset), y = yOffset-r; y <= yOffset + r; y++)
		{
			MoveToEx(hDC, x1, y, NULL);
			LineTo  (hDC, x2, y);
			if (y<yOffset) x1--, x2++;
			else     x1++, x2--;
		}
		break;

	case BS_CIRCLE:
		Arc(hDC, xOffset-r, yOffset-r, xOffset+r+1, yOffset+r+1, xOffset, 0, xOffset, 0);
		break;

	case BS_SATURN:
		r -= 1;
		MoveToEx(hDC, xOffset + r, yOffset + r, NULL);
		LineTo  (hDC, xOffset + r, yOffset - r);
		LineTo  (hDC, xOffset - r, yOffset - r);
		LineTo  (hDC, xOffset - r, yOffset + r);
		LineTo  (hDC, xOffset + r, yOffset + r);
		r += 2;
		MoveToEx(hDC, xOffset + r, yOffset + r, NULL);
		LineTo  (hDC, xOffset + r, yOffset - r);
		LineTo  (hDC, xOffset - r, yOffset - r);
		LineTo  (hDC, xOffset - r, yOffset + r);
		LineTo  (hDC, xOffset + r, yOffset + r);
		break;

	case BS_JUPITER:
		r += 1;
		MoveToEx(hDC, xOffset + r, yOffset + r, NULL);
		LineTo  (hDC, xOffset + r, yOffset - r);
		LineTo  (hDC, xOffset - r, yOffset - r);
		LineTo  (hDC, xOffset - r, yOffset + r);
		LineTo  (hDC, xOffset + r, yOffset + r);
		r -= 1;
		MoveToEx(hDC, xOffset + r + 1, yOffset + r + 1, NULL);
		LineTo  (hDC, xOffset + r + 1, yOffset - r + 1);
		LineTo  (hDC, xOffset - r + 1, yOffset - r + 1);
		LineTo  (hDC, xOffset - r + 1, yOffset + r + 1);
		LineTo  (hDC, xOffset + r + 1, yOffset + r + 1);
		r -= 1;
		MoveToEx(hDC, xOffset + r + 2, yOffset + r + 2, NULL);
		LineTo  (hDC, xOffset + r + 2, yOffset - r + 2);
		LineTo  (hDC, xOffset - r + 2, yOffset - r + 2);
		LineTo  (hDC, xOffset - r + 2, yOffset + r + 2);
		LineTo  (hDC, xOffset + r + 2, yOffset + r + 2);
		break;

	case BS_MARS:
		MoveToEx(hDC, xOffset + 3, yOffset + 3, NULL); //右下
		LineTo  (hDC, xOffset + 3, yOffset - 3); // 右上
		LineTo  (hDC, xOffset - 3, yOffset - 3); // 左上
		LineTo  (hDC, xOffset - 3, yOffset - 1);
		LineTo  (hDC, xOffset + 1, yOffset -1);
		LineTo  (hDC, xOffset + 1, yOffset + 3);
		LineTo  (hDC, xOffset + 3, yOffset + 3);
		MoveToEx(hDC, xOffset - 3, yOffset + 3, NULL);
		LineTo  (hDC, xOffset - 1, yOffset + 3);
		LineTo  (hDC, xOffset - 1, yOffset + 1);
		LineTo  (hDC, xOffset - 3, yOffset + 1);
		LineTo  (hDC, xOffset - 3, yOffset + 3);
		break;

	case BS_VENUS:
		MoveToEx(hDC, xOffset + 1, yOffset + 3, NULL);
		LineTo  (hDC, xOffset + 1, yOffset + 1);
		LineTo  (hDC, xOffset + 3, yOffset + 1);
		LineTo  (hDC, xOffset + 3, yOffset - 1);
		LineTo  (hDC, xOffset + 1, yOffset - 1);
		LineTo  (hDC, xOffset + 1, yOffset - 3);
		LineTo  (hDC, xOffset - 1, yOffset - 3);
		LineTo  (hDC, xOffset - 1, yOffset - 1);
		LineTo  (hDC, xOffset - 3, yOffset - 1);
		LineTo  (hDC, xOffset - 3, yOffset + 1);
		LineTo  (hDC, xOffset - 1, yOffset + 1);
		LineTo  (hDC, xOffset - 1, yOffset + 3);
		LineTo  (hDC, xOffset + 1, yOffset + 3);
		break;

	default:
		break;
	}

	DeleteObject(SelectObject(hDC, oldPen));
}

//===========================================================================
