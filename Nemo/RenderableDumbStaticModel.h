#pragma once
#include <SceneNode.h>
#include <glad/glad.h>

namespace tc
{

class RStaticMesh;
class FGLSLProgram;

class FRenderableDumbStaticModel : public FBaseRenderable
{
    static FGLSLProgram* Program;

    enum BufferObject
    {
        BO_POS = 0,
        BO_NORMAL,
        BO_ELEMENT_ARRAY,
        BO_COUNT
    };

    GLuint VertBuffers[BO_COUNT];
    GLuint VertArray[1];

    bool Initialized;
    int NumElements;
    int ShaderMvpLocation;

    TRefPtr<RStaticMesh> Source;

public:
    FRenderableDumbStaticModel();
    explicit FRenderableDumbStaticModel(RStaticMesh* source);
    ~FRenderableDumbStaticModel() override;
    void InitializeIfNot();
    void Draw(FSceneNode* sceneNode) override;
};

}
