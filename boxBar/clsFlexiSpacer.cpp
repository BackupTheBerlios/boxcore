#include "clsFlexiSpacer.h"

clsFlexiSpacer::clsFlexiSpacer(bool pVertical): clsItem(pVertical)
{
	if (vertical)
		fixed = DIM_HORIZONTAL;
	else
		fixed = DIM_VERTICAL;
}

clsFlexiSpacer::~clsFlexiSpacer()
{
	//dtor
}
