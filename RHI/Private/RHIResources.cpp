#include "RHIResources.h"

namespace tc
{

queue<const FRHIResource*> FRHIResource::PendingDeletionQueue;

void FRHIResource::DeletePending()
{
    while (!PendingDeletionQueue.empty())
    {
        auto* resource = PendingDeletionQueue.front();
        PendingDeletionQueue.pop();
        delete resource;
    }
}

} /* namespace tc */
