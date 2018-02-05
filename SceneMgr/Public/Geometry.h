#pragma once

namespace tc
{

class BoundingBox;

class IGeometry
{
public:
    virtual const BoundingBox& GetBoundingBox() const = 0;

private:
};

} /* namespace tc */
