#pragma once

namespace tc
{

class FEntityManager;
class FPhysicsSystem;

FPhysicsSystem* CreatePhysicsSystem(FEntityManager* mgr);

}
