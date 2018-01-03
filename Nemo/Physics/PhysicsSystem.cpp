#include "Physics.h"
#include "SystemInterface.h"
#include "EntityManager.h"

#include <btBulletDynamicsCommon.h>

namespace tc
{

class FSystem : public ISystem
{
public:
    void Init(FEntityManager* manager) override;
    void Shutdown() override;
    void NotifyEntityAddedToComponent(const FEntityRef& entity, IComponent* component) override;
    void NotifyEntityRemovedFromComponent(const FEntityRef& entity, IComponent* component) override;
    void Tick(int deltaMsec) override {}
};

void FSystem::Init(FEntityManager* manager)
{

}

void FSystem::Shutdown()
{

}

void FSystem::NotifyEntityAddedToComponent(const FEntityRef& entity, IComponent* component)
{

}

void FSystem::NotifyEntityRemovedFromComponent(const FEntityRef& entity, IComponent* component)
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

btRigidBody*FPhysicsSystem::CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
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
}

void FPhysicsSystem::Init(FEntityManager* manager)
{
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
    float deltaSec = deltaMsec / 1000.0f;
    DynamicsWorld->stepSimulation(deltaSec);
}

FPhysicsSystem* CreatePhysicsSystem(FEntityManager* mgr)
{
    return new FPhysicsSystem();
}

}
