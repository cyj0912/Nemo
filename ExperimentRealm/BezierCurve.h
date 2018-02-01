#pragma once
#include "RenderComponent.h"
#include "Shader.h"
#include "EntityLibrary.h"
#include "InteractionSystem.h"

#include "OpenGL.h"

#include <Foundation.h>
#include <Matrix4.h>

namespace tc
{

class FBezierCurveControlPointPrimitive : public FBaseEntity,
                                          public IRenderComponent,
                                          public IInteractionComponent
{
public:
    FBezierCurveControlPointPrimitive();

    const char* GetTypeNameInString() const override;

    size_t CountSubEntities() const override;

    FBaseEntity* GetSubEntity(size_t index) override;

    void RenderInit(FViewPort* rw) override;

    void Render() override;

    void RenderDestroy() override;

    Vector3 InterpolatePosition(const FBezierCurveControlPointPrimitive& rhs, float t) const;

    float GetDistance(const FBezierCurveControlPointPrimitive& rhs) const;

    void CreateGizmo(FInteractionSystem* system) override
    {
        //TODO: Do we really create gizmo?
    }

    IRayIntersectComponent* GetRayIntersectComponent() override
    {
        return &IntersectionTester;
    }

    void ImGuiUpdate(FInteractionSystem* interactionSystem) override;

private:
    FPointPrimitive FrontPoint;
    FPointPrimitive MiddlePoint;
    FPointPrimitive BackPoint;

    class IntersectionTester : public IRayIntersectComponent
    {
    public:
        float RayHitDistance(const Ray& ray) override;
        FBezierCurveControlPointPrimitive* Owner;
    } IntersectionTester;
};

class FBezierCurveRenderComponentStaticData
{
public:
    FBezierCurveRenderComponentStaticData();

    ~FBezierCurveRenderComponentStaticData();

    static void RenderStaticInit();

    static void RenderStaticDestroy();

protected:
    static int UserCount;
    static bool bInitialized;
    static GLuint Buffers[1];
    static GLuint VertexArrays[1];
    static FGLSLProgram* Shader;
};

template <typename TOwner>
class TBezierCurveRenderComponent : public IRenderComponent, public FBezierCurveRenderComponentStaticData
{
public:
protected:
};

class FBezierCurve : public FBaseEntity, public TBezierCurveRenderComponent<FBezierCurve>
{
public:
    FBezierCurve() : ViewPort(nullptr) {}

    void RenderInit(FViewPort* rw) override
    {
        ViewPort = rw;
    }

    void RenderBezierSegment(FBezierCurveControlPointPrimitive* cp0, FBezierCurveControlPointPrimitive* cp1)
    {
        float dist = cp0->GetDistance(*cp1);
        int numLineSegments = (int)(dist * 4.0f);
        float tStep = 1.0f / (float)numLineSegments;
        for (int i = 0; i < numLineSegments; i++)
        {
            auto lineFrom = cp0->InterpolatePosition(*cp1, tStep * (float)i);
            auto lineTo = cp0->InterpolatePosition(*cp1, tStep * (float)(i + 1));
            GEditorMaster->GetPrimitiveRenderer()->DrawLine(lineFrom, lineTo, 1.0f, Color::MAGENTA);
        }
    }

    void Render() override
    {
        size_t nSegments = ControlPoints.size() - 1;
        for (auto i = 0; i < nSegments; i++)
        {
            RenderBezierSegment(ControlPoints[i], ControlPoints[i + 1]);
        }
    }

    void RenderDestroy() override
    {
    }

    void AddControlPoint(FBezierCurveControlPointPrimitive* cp)
    {
        ControlPoints.push_back(cp);
    }

    void AddControlPointFront(FBezierCurveControlPointPrimitive* cp)
    {
        ControlPoints.push_front(cp);
    }

private:
    FViewPort* ViewPort;
    deque<FBezierCurveControlPointPrimitive*, std::allocator<FBezierCurveControlPointPrimitive*>> ControlPoints;
};

} /* namespace tc */
