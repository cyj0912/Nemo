#pragma once

namespace tc
{

class FBaseEntity
{
public:
    virtual ~FBaseEntity();
    virtual const char* GetTypeNameInString() const;
};

} /* namespace tc */
