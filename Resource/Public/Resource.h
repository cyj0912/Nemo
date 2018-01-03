#pragma once
#include "ResourceAPI.h"
#include <Foundation.h>

namespace tc
{

class RESOURCE_API RResource : public FRefCount
{
	TRefPtr<RResource> Parent;
protected:
	RResource() = default;
public:
    ~RResource() override = default;
	RResource* GetParent() const;
	void SetParent(RResource *parent);
};

}
