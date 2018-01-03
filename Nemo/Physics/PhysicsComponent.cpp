#include "Component.h"
#include <bullet/btBulletDynamicsCommon.h>

namespace tc
{

struct FBulletComponentData
{
    int test;
};

class FBulletComponent : public TComponent<FBulletComponentData>
{
public:
};

}
