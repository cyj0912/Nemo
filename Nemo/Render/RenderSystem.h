#pragma once
#include "SystemBase.h"

namespace tc
{

class FRenderSystem : public FSystem
{
public:
    void Init(FEntityManager* manager) override;
    void Shutdown() override;
    void Tick(int deltaMsec) override;
};

}
