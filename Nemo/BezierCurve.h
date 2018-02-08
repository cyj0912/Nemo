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

//Not in the right place. Fix later.
/**
 * Defines cross section(s) in the xy plane.
 */
class FCrossSection
{
public:

private:
};

class FBezierCurveControlPointPrimitive : public FBaseEntity,
                                          public IRenderComponent,
                                          public IInteractionComponent,
                                          public IPropertyChangeListener
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

    void OnPropertyChanged(IPropertyOwner* who, int which) override;

private:
    FPointPrimitive FrontPoint;
    FPointPrimitive MiddlePoint;
    Vector3 PrevMiddlePointPosition;
    FPointPrimitive BackPoint;
    bool bLockTangent;

    //Don't inherit this interface so that EditorMaster doesn't find out about it
    class IntersectionTester : public IRayIntersectComponent
    {
    public:
        float RayHitDistance(const Ray& ray) override;
        FBezierCurveControlPointPrimitive* Owner;
    } IntersectionTester;
};

class FBezierSpline : public FBaseEntity,
                     public IRenderComponent,
                     public IInteractionComponent
{
public:
    FBezierSpline();

    ~FBezierSpline() override;

    const char* GetTypeNameInString() const override;

    void RenderInit(FViewPort* rw) override
    {
        ViewPort = rw;
    }

    void RenderBezierSegment(FBezierCurveControlPointPrimitive* cp0, FBezierCurveControlPointPrimitive* cp1);

    void GenerateSweepMesh(FBezierCurveControlPointPrimitive* cp0, FBezierCurveControlPointPrimitive* cp1);

    void GenerateSweepFrames();

    void Render() override;

    void RenderDestroy() override
    {
    }

    void AddControlPoint(FBezierCurveControlPointPrimitive* cp)
    {
        auto iter = std::find(ControlPoints.begin(), ControlPoints.end(), cp);
        if (iter != ControlPoints.end())
            assert(0 == "Adding the same control point");
        ControlPoints.push_back(cp);
    }

    void AddControlPointFront(FBezierCurveControlPointPrimitive* cp)
    {
        auto iter = std::find(ControlPoints.begin(), ControlPoints.end(), cp);
        if (iter != ControlPoints.end())
            assert(0 == "Adding the same control point");
        ControlPoints.push_front(cp);
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
    bool bAutoNumSegment;
    int NumSegments;

    struct FSweepFrame
    {
        FBezierCurveControlPointPrimitive* ControlPoint0;
        FBezierCurveControlPointPrimitive* ControlPoint1;
        Matrix3x4 Basis;
        float VarT;
    };
    PODVector<FSweepFrame> SweepFrames;

    //Don't inherit this interface so that EditorMaster doesn't find out about it
    class IntersectionTester : public IRayIntersectComponent
    {
    public:
        float RayHitDistance(const Ray& ray) override;
        FBezierSpline* Owner;
    } IntersectionTester;
};

} /* namespace tc */
