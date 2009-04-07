/*
 * clsDropTarget.cpp
 *
 *  Created on: 17 Jan 2009
 *      Author: shaun
 */

#include "clsDropTarget.h"

namespace Plugin_boxBar
{

DropTarget::DropTarget(Item *p_item, DragAction p_drag)
{
	m_item = p_item;
	m_dragAction = p_drag;
	m_refCount = 1;
}

DropTarget::~DropTarget()
{
	// TODO Auto-generated destructor stub
}

HRESULT DropTarget::QueryInterface(REFIID iid, void** ppvObject)
{
	*ppvObject = NULL;
	if (IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IDropTarget))
	{
		*ppvObject = this;
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

ULONG DropTarget::AddRef()
{
	return ++m_refCount;
}

ULONG DropTarget::Release()
{
	int tempCount = --m_refCount;
	if (tempCount==0)
	{
		delete this;
	}
	return tempCount;
}

HRESULT DropTarget::DragEnter(LPDATAOBJECT pDataObject, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	AddRef();
	*pdwEffect  = DROPEFFECT_NONE;
	m_dragAction(m_item, DRAG_ENTER, 0, 0);
	return S_OK;
}

HRESULT DropTarget::DragOver(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	m_dragAction(m_item, DRAG_OVER, pt.x, pt.y);
	*pdwEffect = DROPEFFECT_NONE;
	return S_OK;
}

HRESULT DropTarget::DragLeave()
{
	m_dragAction(m_item, DRAG_LEAVE, 0, 0);
	Release();
	return S_OK;
}

HRESULT DropTarget::Drop(LPDATAOBJECT pDataObject, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	*pdwEffect = DROPEFFECT_NONE;
	m_dragAction(m_item, DRAG_DROP, 0, 0);
	Release();
	return S_OK;
}

}

