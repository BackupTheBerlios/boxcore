#ifndef CLSLABELITEM_H
#define CLSLABELITEM_H

#include "clsItemCollection.h"
#include "clsText.h"

namespace Plugin_boxBar
{

class clsLabelItem : public Collection
{
public:
	clsLabelItem(bool pVertical, LPCSTR p_itemName = "Label");

	//virtual void calculateSizes(bool pSizeGiven = false);
protected:
	Text *m_textItem;
private:
};

}

#endif // CLSLABELITEM_H
