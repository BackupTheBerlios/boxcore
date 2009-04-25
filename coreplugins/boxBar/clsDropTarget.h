/*
 * clsDropTarget.h
 *
 *  Created on: 17 Jan 2009
 *      Author: shaun
 */

#ifndef CLSDROPTARGET_H_
#define CLSDROPTARGET_H_
#include <shlobj.h>

namespace boxBar
{

enum eDragDropState {DRAG_ENTER, DRAG_OVER, DRAG_LEAVE, DRAG_DROP};

class Item;

typedef void (*DragAction)(Item *item, eDragDropState p_state, INT p_x, INT p_y);

class DropTarget : public IDropTarget
{
public:
	DropTarget(Item *p_item, DragAction p_drag);
	virtual ~DropTarget();

	STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD (DragEnter) (LPDATAOBJECT pDataObject, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragOver)(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragLeave)();
	STDMETHOD(Drop)(LPDATAOBJECT pDataObject, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
private:
	DWORD m_refCount;
	DragAction m_dragAction;
	Item *m_item;
};

}

#endif /* CLSDROPTARGET_H_ */
