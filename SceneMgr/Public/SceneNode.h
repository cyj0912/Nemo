#pragma once
#include "Node.h"

namespace tc
{

class IGeometry;

class SCENEMGR_API FSceneNode: public FNode
{
public:
    using FGeometryList = vector<IGeometry*>;
    using FGeometryListIter = vector<IGeometry*>::iterator;
    using FGeometryListConstIter = vector<IGeometry*>::const_iterator;

    FSceneNode();

    FGeometryListIter GeometryBegin()
    {
        return AttachedGeometries.begin();
    }

    FGeometryListIter GeometryEnd()
    {
        return AttachedGeometries.end();
    }

    FGeometryListConstIter GeometryBegin() const
    {
        return AttachedGeometries.begin();
    }

    FGeometryListConstIter GeometryEnd() const
    {
        return AttachedGeometries.end();
    }

private:
    vector<IGeometry*> AttachedGeometries;
};

}
