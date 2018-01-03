#include "NodeFactory.h"
#include "NemoSceneNode.h"

namespace tc
{

FNemoSceneNode *FNodeFactory::NewSceneNode(const string &name)
{
    if (name == "FNemoSceneNode")
        return new FNemoSceneNode();
    else if (name == "FBSplineControl")
        return new FBSplineControl();
    return nullptr;
}

}
