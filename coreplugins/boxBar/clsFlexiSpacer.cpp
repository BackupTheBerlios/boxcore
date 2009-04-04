#include "clsFlexiSpacer.h"

namespace boxBar
{

clsFlexiSpacer::clsFlexiSpacer(bool pVertical): clsItem(pVertical)
{
	if (vertical)
	{
		m_knowsSize = DIM_NONE;
		m_wantsStretch = DIM_VERTICAL;
	}
	else
	{
		m_knowsSize = DIM_NONE;
		m_wantsStretch = DIM_HORIZONTAL;
	}
}

clsFlexiSpacer::~clsFlexiSpacer()
{
	//dtor
}

}
