#pragma once
#include <UsingStl.h>

namespace tc
{

class FBaseEntity;

class FEntityManager
{
public:
    ~FEntityManager();

    void RegisterEntity(FBaseEntity* entity);

    template <typename T> set<T*> GetComponents()
    {
        set<T*> result;
        for (FBaseEntity* ent : Entities)
        {
            if (dynamic_cast<T*>(ent))
                result.insert(dynamic_cast<T*>(ent));
        }
        return result;
    }

    template <typename T> set<FBaseEntity*> GetEntities()
    {
        set<FBaseEntity*> result;
        for (FBaseEntity* ent : Entities)
        {
            if (dynamic_cast<T*>(ent))
                result.insert(ent);
        }
        return result;
    }

    template <typename T1, typename T2> set<FBaseEntity*> GetEntities()
    {
        set<FBaseEntity*> result;
        for (FBaseEntity* ent : Entities)
        {
            if (dynamic_cast<T1*>(ent) && dynamic_cast<T2*>(ent))
                result.insert(ent);
        }
        return result;
    }

private:
    set<FBaseEntity*> Entities;
};

} /* namespace tc */
