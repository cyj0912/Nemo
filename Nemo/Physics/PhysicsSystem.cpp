#include "Physics.h"
#include "SystemBase.h"
#include "EntityManager.h"
#include "ComponentInterface.h"
#include "ComponentStaticRegister.h"

#include <Foundation.h>

#include <btBulletDynamicsCommon.h>

namespace tc
{

void FSystem::TrackComponent(int id)
{
    int i = 0;
    while (TrackedComponents[i] != 0)
        i++;
    TrackedComponents[i] = id;
    TrackedComponentLookup.insert(id);
}

void FSystem::TrackComponent(std::string name)
{
    TrackComponent(Manager->FindComponentInfo(name)->Id);
}

bool FSystem::IsComponentTracked(IComponent* component)
{
    auto iter = TrackedComponentLookup.find(component->GetId());
    return iter != TrackedComponentLookup.end();
}

uint32_t FSystem::GetComponentBit(IComponent *component)
{
    int i;
    for (i = 0; i < 32; i++)
        if (TrackedComponents[i] == component->GetId())
            return (uint32_t)1 << (uint32_t)i;
    return 0;
}

uint32_t FSystem::GetTrackedBitset()
{
    uint32_t result = 0;
    int i;
    for (i = 0; i < 32; i++)
        if (TrackedComponents[i] != 0)
            result |= (uint32_t)1 << (uint32_t)i;
    return result;
}

void FSystem::Init(FEntityManager* manager)
{
    Manager = manager;
    memset(TrackedComponents, 0, sizeof(TrackedComponents));
}

void FSystem::Shutdown()
{
}

void FSystem::NotifyEntityAddedToComponent(const FEntityRef& entity, IComponent* component)
{
    assert(entity.IsValid());
    if (IsComponentTracked(component))
    {
        LOGDEBUG("Sstm Entity %d AddedToComp %d %s\n", entity.GetId(), component->GetId(),
                 FEntityManager::FindComponentInfo(component->GetId())->Name);

        uint32_t bit = GetComponentBit(component);

        auto iter = EntToComp.find(entity.GetId());
        if (iter == EntToComp.end())
            EntToComp[entity.GetId()] = bit;
        else
            EntToComp[entity.GetId()] |= bit;
    }
}

void FSystem::NotifyEntityRemovedFromComponent(const FEntityRef& entity, IComponent* component)
{
    assert(entity.IsValid());
    if (IsComponentTracked(component))
    {
        LOGDEBUG("Sstm Entity %d Removed %d %s\n", entity.GetId(), component->GetId(),
                 FEntityManager::FindComponentInfo(component->GetId())->Name);

        uint32_t bit = GetComponentBit(component);

        EntToComp[entity.GetId()] &= ~bit;
        if (EntToComp[entity.GetId()] == 0)
            EntToComp.erase(entity.GetId());
    }
}

void FSystem::Tick(int deltaMsec)
{
}

class FPhysicsSystem : public FSystem
{
    btBroadphaseInterface* Broadphase;
    btDefaultCollisionConfiguration* CollisionConfiguration;
    btCollisionDispatcher* Dispatcher;
    btDiscreteDynamicsWorld* DynamicsWorld;
    btConstraintSolver* Solver;

    btAlignedObjectArray<btCollisionShape*> ManagedShapes;

protected:
    void CreateEmptyDynamicsWorld();
    btRigidBody* CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);

public:
    void Init(FEntityManager* manager) override;
    void Shutdown() override;
    void Tick(int deltaMsec) override;
};

void FPhysicsSystem::CreateEmptyDynamicsWorld()
{
    CollisionConfiguration = new btDefaultCollisionConfiguration();
    Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
    Broadphase = new btDbvtBroadphase();
    Solver = new btSequentialImpulseConstraintSolver();
    DynamicsWorld = new btDiscreteDynamicsWorld(Dispatcher, Broadphase, Solver, CollisionConfiguration);
    DynamicsWorld->setGravity(btVector3(0, -10, 0));
}

btRigidBody* FPhysicsSystem::CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
{
    assert(!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE);

    bool bIsDynamic = mass != 0.f;

    btVector3 localInertia(0., 0., 0.);
    if (bIsDynamic)
        shape->calculateLocalInertia(mass, localInertia);

    btDefaultMotionState* ms = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo consInfo(mass, ms, shape, localInertia);

    btRigidBody* body = new btRigidBody(consInfo);
    body->setUserIndex(-1);
    DynamicsWorld->addRigidBody(body);

	return body;
}

void FPhysicsSystem::Init(FEntityManager* manager)
{
    FSystem::Init(manager);
    TrackComponent("Physics");

    CreateEmptyDynamicsWorld();

    // have some fun
    btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
    ManagedShapes.push_back(groundShape);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, -50, 0));

    CreateRigidBody(0.f, groundTransform, groundShape);

    btBoxShape* boxShape = new btBoxShape(btVector3(.1, .1, .1));
    ManagedShapes.push_back(boxShape);

    btTransform startTransform;
    startTransform.setIdentity();

    for (int k = 0; k < 10; k++)
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
            {
                startTransform.setOrigin(btVector3(0.2 * i, 0.2 * k, 0.2 * j));
                CreateRigidBody(1.f, startTransform, boxShape);
            }
}

void FPhysicsSystem::Shutdown()
{
    for (int i = 0; i < ManagedShapes.size(); i++)
    {
        auto* shape = ManagedShapes[i];
        delete shape;
    }
    ManagedShapes.clear();

    delete DynamicsWorld;
    DynamicsWorld = nullptr;
    delete Solver;
    Solver = nullptr;
    delete Broadphase;
    Broadphase = nullptr;
    delete Dispatcher;
    Dispatcher = nullptr;
    delete CollisionConfiguration;
    CollisionConfiguration = nullptr;
}

void FPhysicsSystem::Tick(int deltaMsec)
{
    FSystem::Tick(deltaMsec);
    float deltaSec = deltaMsec / 1000.0f;
    DynamicsWorld->stepSimulation(deltaSec);
}

FPhysicsSystem* CreatePhysicsSystem(FEntityManager* mgr)
{
    auto* system = new FPhysicsSystem();
    mgr->RegisterSystem(system);
    return system;
}

}
