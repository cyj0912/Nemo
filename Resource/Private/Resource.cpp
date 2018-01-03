#include "Resource.h"

using namespace tc;

RResource* RResource::GetParent() const
{
    return Parent.Get();
}

void RResource::SetParent(RResource *parent)
{
	Parent = parent;
}
