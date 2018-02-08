#pragma once
#include "EntityLibrary.h"
#include "Shader.h"

namespace tc
{

class FStaticMesh : public FBaseEntity, public FTransformComponent, public IRenderComponent
{
public:
    const char* GetTypeNameInString() const override;

    int CountVertices() const
    {
        return (int)VertexBuffer.size() / 3;
    }

    void AddTriangle(const Vector3& a, const Vector3& b, const Vector3& c);

    void AddQuad(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d);

    // Public mesh data
    vector<float> VertexBuffer; //x, y, z
    vector<float> NormalBuffer; //x, y, z

private:

    /*
     * Rendering component
     * Should be a class in its own right
     */
public:
    void RenderInit(FViewPort* vp) override;

    void RenderReupload();

    void Render() override;

    void RenderDestroy() override;

private:
    using TOwner = FStaticMesh;

    enum
    {
        VB_POSITION,
        VB_NORMAL,
        VB_COUNT
    };

    enum
    {
        VA_MESH,
        VA_COUNT
    };

    GLuint Buffers[VB_COUNT];
    GLuint VertexArrays[VA_COUNT];
    FGLSLProgram* Shader;

    FViewPort* ViewPort;
};

} /* namespace tc */
