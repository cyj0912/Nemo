#pragma once
#include "RenderComponent.h"
#include "Shader.h"
#include "EntityLibrary.h"
#include "InputHandler.h"
#include <UsingStl.h>
#include <cstdint>

namespace tc
{

enum class EAxis : uint8_t
{
    None,
    X,
    Y,
    Z
};

template <typename TOwner>
class TTranslateGizmoRenderComponent : public IRenderComponent
{
public:
    TTranslateGizmoRenderComponent() : RenderWorld(nullptr), Shader(nullptr) {}

    void RenderInit(FViewPort* rw) override
    {
        RenderWorld = rw;

        float coneVertices[] = {
                0.000000f, -1.000000f, -1.000000f,
                0.000000f, 1.000000f, 0.000000f,
                0.866025f, -1.000000f, -0.500000f,
                0.866025f, -1.000000f, 0.500000f,
                -0.000000f, -1.000000f, 1.000000f,
                -0.866025f, -1.000000f, 0.500000f,
                -0.866025f, -1.000000f, -0.500000f
        };

        unsigned short coneIndices[] = {
                0, 1, 2,
                2, 1, 3,
                3, 1, 4,
                4, 1, 5,
                5, 1, 6,
                6, 1, 0,
                0, 2, 3,
                0, 3, 4,
                0, 4, 5,
                0, 5, 6
        };

        float lineVertices[] = {
                0.0, 0.0, 0.0,
                0.0, 1.0, 0.0
        };

        glGenBuffers(3, Buffers);
        glGenVertexArrays(2, VertexArrays);

        //Cone
        glBindVertexArray(VertexArrays[0]);

        glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coneVertices), coneVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(coneIndices), coneIndices, GL_STATIC_DRAW);

        //Line
        glBindVertexArray(VertexArrays[1]);

        glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindVertexArray(0);

        Shader = FGLSLProgram::CreateFromFiles("fgizmocolor.vert", "fgizmocolor.frag");
    }

    void Render() override
    {
        Matrix4 vpMatrix = RenderWorld->GetProjectionMatrix() * RenderWorld->GetViewMatrix();

        Shader->Enable();

        {
            //Y axis
            if (static_cast<TOwner*>(this)->GetHighlightAxis() == EAxis::Y)
                Shader->SetUniform3f("uColor", 0.5f, 1.0f, 0.5f);
            else
                Shader->SetUniform3f("uColor", 0.0f, 0.77f, 0.0f);
            Matrix3x4 localMatrix = Matrix3x4(Vector3(0.0f, 1.0f, 0.0f), Quaternion::IDENTITY, Vector3(0.1f, 0.1f, 0.1f));
            Matrix3x4 modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            Matrix4 mvp = vpMatrix * modelMatrix;

            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[0]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
            glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_SHORT, nullptr);

            localMatrix = Matrix3x4::IDENTITY;
            modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            mvp = vpMatrix * modelMatrix;
            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[1]);
            glDrawArrays(GL_LINES, 0, 2);
            glBindVertexArray(0);
        }
        {
            //X axis
            if (static_cast<TOwner*>(this)->GetHighlightAxis() == EAxis::X)
                Shader->SetUniform3f("uColor", 1.0, 0.5, 0.5);
            else
                Shader->SetUniform3f("uColor", 1.0, 0.0, 0.0);
            Matrix3x4 localMatrix = Matrix3x4(Vector3(1.0f, 0.0f, 0.0f), Quaternion(Vector3::UNIT_Y, Vector3::UNIT_X), Vector3(0.1f, 0.1f, 0.1f));
            Matrix3x4 modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            Matrix4 mvp = vpMatrix * modelMatrix;

            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[0]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
            glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_SHORT, nullptr);

            localMatrix = Matrix3x4(Vector3::ZERO, Quaternion(Vector3::UNIT_Y, Vector3::UNIT_X), Vector3::ONE);
            modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            mvp = vpMatrix * modelMatrix;
            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[1]);
            glDrawArrays(GL_LINES, 0, 2);
            glBindVertexArray(0);
        }
        {
            //Z axis
            if (static_cast<TOwner*>(this)->GetHighlightAxis() == EAxis::Z)
                Shader->SetUniform3f("uColor", 0.5, 0.5, 1.0);
            else
                Shader->SetUniform3f("uColor", 0.0, 0.0, 1.0);
            Matrix3x4 localMatrix = Matrix3x4(Vector3(0.0f, 0.0f, 1.0f), Quaternion(Vector3::UNIT_Y, Vector3::UNIT_Z), Vector3(0.1f, 0.1f, 0.1f));
            Matrix3x4 modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            Matrix4 mvp = vpMatrix * modelMatrix;

            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[0]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
            glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_SHORT, nullptr);

            localMatrix = Matrix3x4(Vector3::ZERO, Quaternion(Vector3::UNIT_Y, Vector3::UNIT_Z), Vector3::ONE);
            modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            mvp = vpMatrix * modelMatrix;
            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[1]);
            glDrawArrays(GL_LINES, 0, 2);
            glBindVertexArray(0);
        }
    }

    void RenderDestroy() override
    {
        delete Shader;
        glDeleteBuffers(3, Buffers);
        glDeleteVertexArrays(2, VertexArrays);
    }

private:
    FViewPort* RenderWorld;
    GLuint Buffers[3]{}, VertexArrays[2]{};
    FGLSLProgram* Shader;
};

class FTranslateGizmo : public FBaseEntity, public TTranslateGizmoRenderComponent<FTranslateGizmo>
{
public:
    explicit FTranslateGizmo(FNode* trackedTransform)
            : TransformNode(trackedTransform), HighlightAxis(EAxis::None)
    {
        TransformNode->AddRef();
    }

    FNode& GetTransformNode()
    {
        return *TransformNode;
    }

    const FNode& GetTransformNode() const
    {
        return *TransformNode;
    }

    const Matrix3x4& GetTransformMatrix() const
    {
        return TransformNode->GetTransformToWorld();
    }

    EAxis HitWorldSpaceRay(const Ray& worldRay);

    EAxis GetHighlightAxis() const
    {
        return HighlightAxis;
    }

    void SetHighlightAxis(EAxis value)
    {
        HighlightAxis = value;
    }

    Ray GetAxisRayWorldSpace(EAxis axis);

private:
    // Component: Transform
    FNode* TransformNode;

    // Data (Shared Data?) (Or What?)
    EAxis HighlightAxis; //1=x 2=y 3=z
};

class FPointTranslateGizmo : public FBaseEntity,
                             public FTransformComponent,
                             public TTranslateGizmoRenderComponent<FPointTranslateGizmo>
{
public:
    explicit FPointTranslateGizmo(Vector3* trackedPoint) : TrackedPoint(trackedPoint), HighlightAxis(EAxis::None)
    {
        SetTransformFromPoint();
    }

    void SetTransformFromPoint()
    {
        GetTransformNode().SetTranslation(*TrackedPoint);
    }

    void MoveAlongAxis(EAxis axis, float offset);

    EAxis HitWorldSpaceRay(const Ray& worldRay);

    EAxis GetHighlightAxis() const
    {
        return HighlightAxis;
    }

    void SetHighlightAxis(EAxis value)
    {
        HighlightAxis = value;
    }

    Ray GetAxisRayWorldSpace(EAxis axis);

private:
    // Data (Shared Data?) (Or What?)
    Vector3* TrackedPoint;
    EAxis HighlightAxis; //1=x 2=y 3=z
};

class FTranslateGizmoInputHandler : public IInputHandler
{
public:
    FTranslateGizmoInputHandler(FTranslateGizmo* gizmo, FViewPort* vp);

    bool MouseMoved(const FMouseMotionEvent& evt) override;

    bool MousePressed(const FMouseButtonEvent& evt) override;

    bool MouseReleased(const FMouseButtonEvent& evt) override;

private:
    FTranslateGizmo* Gizmo;
    FViewPort* ViewPort;

    Ray DragAlongRay;
    EAxis DragAlongAxis;
    bool bDraggingAlong;
    Vector3 LastClosestPoint;
};

class FPointTranslateGizmoInputHandler : public IInputHandler
{
public:
    FPointTranslateGizmoInputHandler(FPointTranslateGizmo* gizmo, FViewPort* vp);

    bool MouseMoved(const FMouseMotionEvent& evt) override;

    bool MousePressed(const FMouseButtonEvent& evt) override;

    bool MouseReleased(const FMouseButtonEvent& evt) override;

private:
    FPointTranslateGizmo* Gizmo;
    FViewPort* ViewPort;

    Ray DragAlongRay;
    EAxis DragAlongAxis;
    bool bDraggingAlong;
    Vector3 LastClosestPoint;

};

} /* namespace tc */
