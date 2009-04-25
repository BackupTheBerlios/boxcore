#ifndef CLSLABELITEM_H
#define CLSLABELITEM_H

#include "clsCollection.h"
#include "clsText.h"

namespace Plugin_boxBar
{

class Label : public Collection
{
public:
	Label(bool pVertical, LPCSTR p_itemName = "Label");
protected:
	Text *m_textItem;
};

}

#endif // CLSLABELITEM_H
