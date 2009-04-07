#ifndef CLSLABELITEM_H
#define CLSLABELITEM_H

#include "clsTextItem.h"

namespace Plugin_boxBar
{

class clsLabelItem : public Text
{
public:
	clsLabelItem(bool pVertical);

	virtual void calculateSizes(bool pSizeGiven = false);
protected:
private:

};

}

#endif // CLSLABELITEM_H
