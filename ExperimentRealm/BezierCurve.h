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

class FStaticMesh;

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

    const FPointPrimitive& GetFrontPoint() const
    {
        return FrontPoint;
    }

    const FPointPrimitive& GetMiddlePoint() const
    {
        return MiddlePoint;
    }

    const FPointPrimitive& GetBackPoint() const
    {
        return BackPoint;
    }

private:
    FPointPrimitive FrontPoint;
    FPointPrimitive MiddlePoint;
    FPointPrimitive BackPoint;

    //Don't inherit this interface so that EditorMaster doesn't find out about it
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

class FBezierCurve : public FBaseEntity,
                     public TBezierCurveRenderComponent<FBezierCurve>,
                     public IInteractionComponent
{
public:
    FBezierCurve();

    ~FBezierCurve() override;

    const char* GetTypeNameInString() const override;

    void RenderInit(FViewPort* rw) override
    {
        ViewPort = rw;
    }

    void RenderBezierSegment(FBezierCurveControlPointPrimitive* cp0, FBezierCurveControlPointPrimitive* cp1);

    void GenerateSweepMesh(FBezierCurveControlPointPrimitive* cp0, FBezierCurveControlPointPrimitive* cp1);

    void Render() override;

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

    void CreateGizmo(FInteractionSystem* system) override
    {
        //No gizmo
    }

    IRayIntersectComponent* GetRayIntersectComponent() override
    {
        return &IntersectionTester;
    }

    void ImGuiUpdate(FInteractionSystem* interactionSystem) override;

private:
    FViewPort* ViewPort;
    deque<FBezierCurveControlPointPrimitive*, std::allocator<FBezierCurveControlPointPrimitive*>> ControlPoints;
    bool bSweep;
    FStaticMesh* SweepMesh;
    bool bShowTan, bShowInward, bShowNormal, bShowBiNormal;

    //Don't inherit this interface so that EditorMaster doesn't find out about it
    class IntersectionTester : public IRayIntersectComponent
    {
    public:
        float RayHitDistance(const Ray& ray) override;
        FBezierCurve* Owner;
    } IntersectionTester;
};

} /* namespace tc */
