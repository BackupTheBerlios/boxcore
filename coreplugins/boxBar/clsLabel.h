#ifndef CLSLABELITEM_H
#define CLSLABELITEM_H

#include "clsCollection.h"
#include "clsText.h"

namespace boxBar
{

class Label : public Collection
{
public:
	Label(bool pVertical, LPCSTR p_itemName = "Label");

	void SetStyle(UINT p_style);
protected:
	Text *m_textItem;
};

}

#endif // CLSLABELITEM_H
