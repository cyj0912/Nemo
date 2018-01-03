#include "ResourceFactory.h"
#include "Resource.h"

namespace tc
{

void FResourceFactory::Init()
{
}

void FResourceFactory::Shutdown()
{
}

TRefPtr<RResource> FResourceFactory::NewResource(string identifier)
{
    return nullptr;
}

FResourceFactory GResourceFactory;

}
