#include "RenderableDumbStaticModel.h"
#include "Shader.h"
#include "NemoRoot.h"
#include <StaticMesh.h>
#include <Scene.h>

namespace tc
{

FGLSLProgram* FRenderableDumbStaticModel::Program = nullptr;

FRenderableDumbStaticModel::FRenderableDumbStaticModel()
    : Initialized(false), NumElements(0), ShaderMvpLocation(-1)
{
}

FRenderableDumbStaticModel::FRenderableDumbStaticModel(RStaticMesh *source) : Initialized(false), Source(source)
{
}

FRenderableDumbStaticModel::~FRenderableDumbStaticModel()
{
    glDeleteBuffers(BO_COUNT, VertBuffers);
    glDeleteVertexArrays(1, VertArray);
}

void FRenderableDumbStaticModel::InitializeIfNot()
{
    if (!Initialized)
    {
        if (!this->Program)
            Program = FGLSLProgram::CreateFromFiles("BasicMVP.vert", "BasicMVP.frag");

        ShaderMvpLocation = Program->GetUniformLocation("uModelViewProjectionMatrix");
        int posAttribLocation = Program->GetAttribLocation("VertexPosition");
        LOGDEBUG("BasicMVP uModelViewProjectionMatrix Location: %d\n", ShaderMvpLocation);
        LOGDEBUG("BasicMVP VertexPosition Location: %d\n", posAttribLocation);

        glGenBuffers(BO_COUNT, VertBuffers);
        glGenVertexArrays(1, VertArray);
        glBindVertexArray(VertArray[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VertBuffers[BO_POS]);
        glBufferData(GL_ARRAY_BUFFER,
                     Source->VertexBuffer.size() * sizeof(float), Source->VertexBuffer.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(posAttribLocation);
        glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, VertBuffers[BO_NORMAL]);
        glBufferData(GL_ARRAY_BUFFER,
                     Source->NormalBuffer.size() * sizeof(float), Source->NormalBuffer.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(BO_NORMAL);
        glVertexAttribPointer(BO_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertBuffers[BO_ELEMENT_ARRAY]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     Source->IndexBuffer.size() * sizeof(unsigned short), Source->IndexBuffer.data(), GL_STATIC_DRAW);

        NumElements = (int)Source->IndexBuffer.size();

    //    float oneCCWTriangle[] = {0.0f, 0.0f, 0.0f,
    //                              1.0f, 0.0f, 0.0f,
    //                              1.0f, 1.0f, 0.0f};
    //    glBindBuffer(GL_ARRAY_BUFFER, VertBuffers[BO_POS]);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(oneCCWTriangle), oneCCWTriangle, GL_STATIC_DRAW);
    //    glEnableVertexAttribArray(posAttribLocation);
    //    glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        Initialized = true;
    }
}

void FRenderableDumbStaticModel::Draw(FSceneNode* sceneNode)
{
    InitializeIfNot();
    LOGDEBUG("FRenderableDumbStaticModel::Draw: %s\n", sceneNode->GetName().c_str());
    LOGINFO("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
    auto camNode = GNemoRoot.GetMainScene()->GetActiveCameraNode();
    FCamera* camera = nullptr;
    if (camNode)
        camera = camNode->GetAttachedCamera();
    if (!camNode || !camera)
    {
        LOGWARN("FRenderableDumbStaticModel::Draw no camera found!\n");
        return;
    }
    auto projMat = camera->GetProjectionMatrix();
    auto viewMat = camNode->GetTransformFromWorld().ToMatrix4();

    auto modelMat = sceneNode->GetTransformToWorld().ToMatrix4();
    auto MVPMat = projMat * viewMat * modelMat;

    Program->Enable();
    Program->SetUniformMatrix4fv(ShaderMvpLocation, MVPMat.Data(), 1, true);

    glBindVertexArray(VertArray[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertBuffers[BO_ELEMENT_ARRAY]);

    glEnable(GL_DEPTH_TEST);

    glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_SHORT, nullptr);
    //glDrawArrays(GL_TRIANGLES, 0, 9);

    //Epilogue
    glBindVertexArray(0);
    Program->Disable();
}

}
