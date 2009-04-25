#include "clsFlexiSpacer.h"

namespace boxBar
{

clsFlexiSpacer::clsFlexiSpacer(): Item()
{
	m_knowsSize = DIM_NONE;
	m_wantsStretch = DIM_BOTH;
}

clsFlexiSpacer::~clsFlexiSpacer()
{
	//dtor
}

}
