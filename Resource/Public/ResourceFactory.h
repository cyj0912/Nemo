#pragma once
#include "ResourceAPI.h"
#include <Foundation.h>
#include <string>

namespace tc
{

class RResource;

class RESOURCE_API FResourceFactory
{
public:
    void Init();
    void Shutdown();

    TRefPtr<RResource> NewResource(string identifier);
};

extern FResourceFactory GResourceFactory;

}
