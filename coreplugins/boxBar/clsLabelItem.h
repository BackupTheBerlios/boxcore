#ifndef CLSLABELITEM_H
#define CLSLABELITEM_H

#include "clsTextItem.h"

class clsLabelItem : public clsTextItem
{
	public:
		clsLabelItem(bool pVertical);

		virtual void calculateSizes(bool pSizeGiven = false);
	protected:
	private:

};

#endif // CLSLABELITEM_H
