#pragma once
#include <Foundation.h>

namespace tc
{

class FNemoSceneNode;

class FNodeFactory
{
public:
    static FNemoSceneNode* NewSceneNode(const string& name);
};

}
