#include "Component.h"
#include "ComponentStaticRegister.h"
#include <bullet/btBulletDynamicsCommon.h>

namespace tc
{

struct FBulletComponentData
{
    int test;
};

class FBulletComponent : public TComponent<FBulletComponentData, FBulletComponent>
{
public:
    static const int Id = 101;

    FBulletComponent(FEntityManager* mgr) : TComponent(mgr)
    {
    }

    void OnEntityAdded(FEntityRef& entity, FBulletComponentData* data)
    {

    }

    void OnEntityToBeRemoved(FEntityRef& entity, FBulletComponentData* data)
    {

    }
};

REGISTER_COMPONENT(Physics, FBulletComponent);

}
