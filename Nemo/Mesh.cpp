#include "Mesh.h"

namespace tc
{

const char* FStaticMesh::GetTypeNameInString() const
{
    static const char* name = "FStaticMesh";
    return name;
}

void FStaticMesh::AddTriangle(const Vector3& a, const Vector3& b, const Vector3& c)
{
    Vector3 surfaceNormal = (c - a).CrossProduct(c - b);
    VertexBuffer.push_back(a.x_); VertexBuffer.push_back(a.y_); VertexBuffer.push_back(a.z_);
    VertexBuffer.push_back(b.x_); VertexBuffer.push_back(b.y_); VertexBuffer.push_back(b.z_);
    VertexBuffer.push_back(c.x_); VertexBuffer.push_back(c.y_); VertexBuffer.push_back(c.z_);
    for (int i = 0; i < 3; i++)
    {
        NormalBuffer.push_back(surfaceNormal.x_);
        NormalBuffer.push_back(surfaceNormal.y_);
        NormalBuffer.push_back(surfaceNormal.z_);
    }
}

void FStaticMesh::RenderInit(FViewPort* vp)
{
    ViewPort = vp;

    glGenBuffers(VB_COUNT, Buffers);
    glGenVertexArrays(VA_COUNT, VertexArrays);

    glBindVertexArray(VertexArrays[VA_MESH]);
    //Position @ 0
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VB_POSITION]);
    glBufferData(GL_ARRAY_BUFFER, VertexBuffer.size() * sizeof(float), VertexBuffer.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    //Normal @ 1
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VB_NORMAL]);
    glBufferData(GL_ARRAY_BUFFER, NormalBuffer.size() * sizeof(float), NormalBuffer.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Shader = FGLSLProgram::CreateFromFiles("fbox.vert", "fbox.frag");
}

void FStaticMesh::RenderReupload()
{
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VB_POSITION]);
    glBufferData(GL_ARRAY_BUFFER, VertexBuffer.size() * sizeof(float), VertexBuffer.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VB_NORMAL]);
    glBufferData(GL_ARRAY_BUFFER, NormalBuffer.size() * sizeof(float), NormalBuffer.data(), GL_DYNAMIC_DRAW);
}

void FStaticMesh::Render()
{
    Matrix4 modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix().ToMatrix4();
    Matrix4 mvp = ViewPort->GetProjectionMatrix() * ViewPort->GetViewMatrix() * modelMatrix;
    Matrix3 viewNoTranslate = ViewPort->GetViewMatrix().ToMatrix3();
    Matrix3 normalTransform = viewNoTranslate * modelMatrix.ToMatrix3();
    normalTransform = normalTransform.Inverse();
    normalTransform = normalTransform.Transpose();

    Vector3 eyeDirLight = viewNoTranslate * Vector3(-0.90f, 0.04f, 0.43f);

    Shader->Enable();
    Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);
    Shader->SetUniformMatrix3fv("uNormalTransform", normalTransform.Data(), 1, true);
    Shader->SetUniform3f("uEyeDirLight", eyeDirLight.x_, eyeDirLight.y_, eyeDirLight.z_);

    glBindVertexArray(VertexArrays[VA_MESH]);
    glDrawArrays(GL_TRIANGLES, 0, CountVertices());
    glBindVertexArray(0);
}

void FStaticMesh::RenderDestroy()
{
    glDeleteBuffers(VB_COUNT, Buffers);
    glDeleteVertexArrays(VA_COUNT, VertexArrays);
}

} /* namespace tc */
