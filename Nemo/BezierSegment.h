#pragma once
#include <SceneAttachment.h>
#include <Matrix4.h>

#include <glad/glad.h>

namespace tc
{

class FBSplineControl;
class FGLSLProgram;

class FBezierSegment : public FBaseRenderable
{
    //Two FBSplineControl collectively owns this
    FBSplineControl* ControlBegin;
    FBSplineControl* ControlEnd;

    enum BufferObject
    {
        BO_POS = 0,
        BO_COUNT
    };

    GLuint VertBuffers[BO_COUNT];
    GLuint VertArray[1];

    static FGLSLProgram* Program;
    int ShaderMvpLocation, PosAttribLocation, ColorLocation;
    int NumSegments;
    bool Initialized;

protected:
    void DrawLineStrip(const float* array, int pointCount, const Matrix4& mvp, const Vector3& color);

public:
    FBezierSegment(FBSplineControl* controlBegin, FBSplineControl* controlEnd);
    ~FBezierSegment() override;
    void InitializeIfNot();
    void Draw(FSceneNode* sceneNode) override;
};

}
