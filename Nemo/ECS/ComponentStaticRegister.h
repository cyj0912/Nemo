#pragma once
#include <UsingStl.h>

namespace tc
{

class IComponent;
struct FComponentInfo;
class FEntityManager;

extern FComponentInfo* ComponentInfoListHead;

struct FComponentInfo
{
    char Name[32];
    int Id;

    typedef IComponent* (*FACTORYFUNC)(FEntityManager* mgr);
    FACTORYFUNC Factory;

    FComponentInfo* ListNext;
};

class FComponentInfoRegHelper
{
public:
    FComponentInfoRegHelper(FComponentInfo* info)
    {
        info->ListNext = ComponentInfoListHead;
        ComponentInfoListHead = info;
    }
};

}

#define REGISTER_COMPONENT(name, class_name) \
    IComponent* name##Factory(FEntityManager* mgr) { return new class_name(mgr); } \
    FComponentInfo name##Info {#name, class_name::Id, name##Factory}; \
    FComponentInfoRegHelper name##InfoHelper {&name##Info}
