#pragma once

namespace tc
{

class FSceneNode;

class FSceneManager
{
public:
    FSceneManager();

    FSceneManager(const FSceneManager& other) = delete;
    FSceneManager(FSceneManager&& other) = delete;
    FSceneManager& operator= (const FSceneManager& other) = delete;
    FSceneManager& operator= (FSceneManager&& other) = delete;

private:
    FSceneNode* RootNode;
};

} /* namespace tc */
