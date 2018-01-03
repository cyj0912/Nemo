#include "BezierSegment.h"
#include "NemoSceneNode.h"
#include "NemoRoot.h"
#include "Shader.h"
#include <Scene.h>

namespace tc
{

//TODO: Move this to Math

Vector3 QuadraticBezierInterp(const Vector3& a, const Vector3& b, const Vector3& c, float t)
{
    Vector3 d = a + t * (b - a);
    Vector3 e = b + t * (c - b);
    return d + t * (e - d);
}

Vector3 CubicBezierInterp(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float t)
{
    Vector3 e = a + t * (b - a);
    Vector3 f = b + t * (c - b);
    Vector3 g = c + t * (d - c);
    return QuadraticBezierInterp(e, f, g, t);
}

FGLSLProgram* FBezierSegment::Program = nullptr;

void FBezierSegment::DrawLineStrip(const float* array, int pointCount, const Matrix4& mvp, const Vector3& color)
{
    glBindVertexArray(VertArray[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VertBuffers[BO_POS]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, pointCount * 3 * sizeof(float), array);

    Program->Enable();
    Program->SetUniformMatrix4fv(ShaderMvpLocation, mvp.Data(), 1, true);
    Program->SetUniform3f(ColorLocation, color.x_, color.y_, color.z_);

    glDrawArrays(GL_LINE_STRIP, 0, pointCount);
    //Epilogue
    glBindVertexArray(0);
    Program->Disable();
}

FBezierSegment::FBezierSegment(FBSplineControl *controlBegin, FBSplineControl *controlEnd)
    : ControlBegin(controlBegin), ControlEnd(controlEnd), NumSegments(16), Initialized(false)
{
}

FBezierSegment::~FBezierSegment()
{
    glDeleteBuffers(BO_COUNT, VertBuffers);
    glDeleteVertexArrays(1, VertArray);
}

void FBezierSegment::InitializeIfNot()
{
    if (!Initialized)
    {
        if (!this->Program)
            Program = FGLSLProgram::CreateFromFiles("BasicMVP.vert", "BasicMVPWhite.frag");

        ShaderMvpLocation = Program->GetUniformLocation("uModelViewProjectionMatrix");
        ColorLocation = Program->GetUniformLocation("uColor");
        PosAttribLocation = Program->GetAttribLocation("VertexPosition");

        glGenBuffers(BO_COUNT, VertBuffers);
        glGenVertexArrays(1, VertArray);
        glBindVertexArray(VertArray[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VertBuffers[BO_POS]);
        glBufferData(GL_ARRAY_BUFFER, (NumSegments + 1) * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(PosAttribLocation);
        glVertexAttribPointer(PosAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        Initialized = true;
    }
}

void FBezierSegment::Draw(FSceneNode* sceneNode)
{
    InitializeIfNot();
    /*
     * Doing all the calculations in world space
     */

    if (sceneNode != ControlBegin)
        return;
    //LOGDEBUG("FBezierSegment::Draw From %s To %s\n", ControlBegin->GetName().c_str(), ControlEnd->GetName().c_str());
    Vector3 a = ControlBegin->GetWorldTranslation();
    Vector3 b = ControlBegin->ConvertVectorTo(ControlBegin->GetFrontPointOffset(), ControlBegin->GetRootNode());
    Vector3 c = ControlEnd->ConvertVectorTo(ControlEnd->GetBackPointOffset(), ControlEnd->GetRootNode());
    Vector3 d = ControlEnd->GetWorldTranslation();
    //LOGDEBUG("a = <%s>\n", a.ToString().c_str());
    //LOGDEBUG("b = <%s>\n", b.ToString().c_str());
    //LOGDEBUG("c = <%s>\n", c.ToString().c_str());
    //LOGDEBUG("d = <%s>\n", d.ToString().c_str());

    float abcd[12];
    memcpy(abcd + 0, a.Data(), 3 * sizeof(float));
    memcpy(abcd + 3, b.Data(), 3 * sizeof(float));
    memcpy(abcd + 6, c.Data(), 3 * sizeof(float));
    memcpy(abcd + 9, d.Data(), 3 * sizeof(float));

    float* vertBuffer = new float[(NumSegments + 1) * 3];
    float t = 0.0f;
    float tStep = 1.0f / (float)NumSegments;
    for (int i = 0; i <= NumSegments; i++)
    {
        Vector3 p = CubicBezierInterp(a, b, c, d, t);
        vertBuffer[i * 3 + 0] = p[0];
        vertBuffer[i * 3 + 1] = p[1];
        vertBuffer[i * 3 + 2] = p[2];
        //LOGDEBUG("p%d = <%s>\n", i, p.ToString().c_str());
        t += tStep;
    }

    // TODO: !!!!!!!! Code duplication !!!!!!!!
    auto camNode = GNemoRoot.GetMainScene()->GetActiveCameraNode();
    FCamera* camera = nullptr;
    if (camNode)
        camera = camNode->GetAttachedCamera();
    if (!camNode || !camera)
    {
        LOGWARN("FBezierSegment::Draw no camera found!\n");
        return;
    }
    auto projMat = camera->GetProjectionMatrix();
    auto viewMat = camNode->GetTransformFromWorld().ToMatrix4();
    auto MVPMat = projMat * viewMat;

    DrawLineStrip(abcd, 2, MVPMat, Vector3(1.0f, 0.0f, 0.0f));
    DrawLineStrip(abcd + 6, 2, MVPMat, Vector3(0.0f, 1.0f, 0.0f));
    DrawLineStrip(vertBuffer, NumSegments + 1, MVPMat, Vector3::ZERO);
	delete[] vertBuffer;
}

}
