#include "BezierCurve.h"
#include "Mesh.h"

#include "imgui/imgui.h"

namespace tc
{

//Some math functions
Vector3 QuadraticBezierInterp(const Vector3& a, const Vector3& b, const Vector3& c, float t)
{
    Vector3 d = a + t * (b - a);
    Vector3 e = b + t * (c - b);
    return d + t * (e - d);
}

/*
 * Not as efficient
Vector3 CubicBezierInterp(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float t)
{
    Vector3 e = a + t * (b - a);
    Vector3 f = b + t * (c - b);
    Vector3 g = c + t * (d - c);
    return QuadraticBezierInterp(e, f, g, t);
}
*/

Vector3 CubicBezierInterp(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float t)
{
    return -a * (-1 + t) * (-1 + t) * (-1 + t) + t * (3 * b * (-1 + t) * (-1 + t) + t * (3 * c - 3 * c * t + d * t));
}

Vector3 CubicBezierPrime(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float t)
{
    return -3 * (a * (-1 + t) * (-1 + t) + t * (-2 * c + 3 * c * t - d * t) + b * (-1 + 4 * t - 3 * t * t));
}

Vector3 CubicBezierDoublePrime(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float t)
{
    return 6 * (a + c - a * t - 3 * c * t + d * t + b * (-2 + 3 * t));
}

//Begin FBezierCurveControlPointPrimitive
FBezierCurveControlPointPrimitive::FBezierCurveControlPointPrimitive()
{
    static int displacement = 0;
    FrontPoint.SetPosition(Vector3(1.0f, 1.0f + displacement, 0.0f));
    MiddlePoint.SetPosition(Vector3(0.0f, 0.0f + displacement, 0.0f));
    BackPoint.SetPosition(Vector3(-1.0f, -1.0f + displacement, 0.0f));
    displacement++;
    displacement %= 5;
    IntersectionTester.Owner = this;
}

const char* FBezierCurveControlPointPrimitive::GetTypeNameInString() const
{
    static const char* name = "FBezierCurveControlPointPrimitive";
    return name;
}

size_t FBezierCurveControlPointPrimitive::CountSubEntities() const
{
    return 3;
}

FBaseEntity* FBezierCurveControlPointPrimitive::GetSubEntity(size_t index)
{
    assert(index < 3);
    switch(index)
    {
    case 0:
        return &BackPoint;
    case 1:
        return &MiddlePoint;
    case 2:
        return &FrontPoint;
    default:
        break;
    }
    return nullptr;
}

void FBezierCurveControlPointPrimitive::RenderInit(FViewPort* rw)
{
    FrontPoint.RenderInit(rw);
    MiddlePoint.RenderInit(rw);
    BackPoint.RenderInit(rw);
}

void FBezierCurveControlPointPrimitive::Render()
{
    FrontPoint.Render();
    MiddlePoint.Render();
    BackPoint.Render();
    GEditorMaster->GetPrimitiveRenderer()->DrawLine(BackPoint.GetPosition(), MiddlePoint.GetPosition(), 1.0f, Color(0.259, 0.431, 0.957));
    GEditorMaster->GetPrimitiveRenderer()->DrawLine(MiddlePoint.GetPosition(), FrontPoint.GetPosition(), 1.0f, Color(0.467, 0.953, 1));
}

void FBezierCurveControlPointPrimitive::RenderDestroy()
{
    FrontPoint.RenderDestroy();
    MiddlePoint.RenderDestroy();
    BackPoint.RenderDestroy();
}

Vector3 FBezierCurveControlPointPrimitive::InterpolatePosition(const FBezierCurveControlPointPrimitive& rhs, float t) const
{
    const auto& p0 = MiddlePoint.GetPosition();
    const auto& p1 = FrontPoint.GetPosition();
    const auto& p2 = rhs.BackPoint.GetPosition();
    const auto& p3 = rhs.MiddlePoint.GetPosition();
    return CubicBezierInterp(p0, p1, p2, p3, t);
}

float FBezierCurveControlPointPrimitive::GetDistance(const FBezierCurveControlPointPrimitive& rhs) const
{
    return (rhs.BackPoint.GetPosition() - FrontPoint.GetPosition()).Length() +
           (rhs.MiddlePoint.GetPosition() - rhs.BackPoint.GetPosition()).Length() +
           (FrontPoint.GetPosition() - MiddlePoint.GetPosition()).Length();
}

void FBezierCurveControlPointPrimitive::ImGuiUpdate(FInteractionSystem* interactionSystem)
{
    if (ImGui::Button("Connect Control Points"))
    {
        if (interactionSystem->GetSelectedEntities().size() == 1)
        {
            auto* rhs = dynamic_cast<FBezierCurveControlPointPrimitive*>(*interactionSystem->GetSelectedEntities().begin());
            if (rhs)
            {
                auto* curve = new FBezierCurve();
                curve->AddControlPoint(this);
                curve->AddControlPoint(rhs);
                GEditorMaster->RegisterEntity(curve);
            }
        }
    }

    if (ImGui::Button("Flip"))
    {
        Vector3 temp = FrontPoint.GetPosition();
        FrontPoint.SetPosition(BackPoint.GetPosition());
        BackPoint.SetPosition(temp);
    }
}

float FBezierCurveControlPointPrimitive::IntersectionTester::RayHitDistance(const Ray& ray)
{
    static const float DISTANCE_THRESHOLD = 0.2f;
    auto testSeg = [=](const Vector3& middle, const Vector3& front)
    {
        const Vector3& origin = middle;
        const Vector3& target = front;
        Vector3 dir = target - origin;
        Ray rayAlongSeg = Ray(origin, dir);
        Vector3 closestPoint = rayAlongSeg.ClosestPoint(ray);
        float distance = ray.Distance(closestPoint);
        if (distance > DISTANCE_THRESHOLD)
            return M_INFINITY;
        Vector3 pointOffset = closestPoint - origin;
        if (pointOffset.DotProduct(dir) < 0)
            return M_INFINITY;
        if (pointOffset.LengthSquared() > dir.LengthSquared())
            return M_INFINITY;
        return distance;
    };
    return Min(testSeg(Owner->MiddlePoint.GetPosition(), Owner->FrontPoint.GetPosition()),
               testSeg(Owner->MiddlePoint.GetPosition(), Owner->BackPoint.GetPosition()));
}

FBezierCurve::FBezierCurve() : ViewPort(nullptr), bSweep(false), SweepMesh(nullptr),
                               bShowTan(false), bShowInward(false), bShowNormal(false), bShowBiNormal(false)
{
    IntersectionTester.Owner = this;
}

const char* FBezierCurve::GetTypeNameInString() const
{
    static const char* name = "FBezierCurve";
    return name;
}

void FBezierCurve::RenderBezierSegment(FBezierCurveControlPointPrimitive* cp0, FBezierCurveControlPointPrimitive* cp1)
{
    float dist = cp0->GetDistance(*cp1);
    int numLineSegments = (int)(dist * 2.0f);
    float tStep = 1.0f / (float)numLineSegments;
    for (int i = 0; i < numLineSegments; i++)
    {
        const auto& p0 = cp0->GetMiddlePoint().GetPosition();
        const auto& p1 = cp0->GetFrontPoint().GetPosition();
        const auto& p2 = cp1->GetBackPoint().GetPosition();
        const auto& p3 = cp1->GetMiddlePoint().GetPosition();
        auto lineFrom = CubicBezierInterp(p0, p1, p2, p3, tStep * (float)i);
        auto lineTo = CubicBezierInterp(p0, p1, p2, p3, tStep * (float)(i + 1));
        GEditorMaster->GetPrimitiveRenderer()->DrawLine(lineFrom, lineTo, 1.0f, Color::MAGENTA);

        auto tangentDir = CubicBezierPrime(p0, p1, p2, p3, tStep * (float)i);
        if (bShowTan)
        {
            auto tangentTo = lineFrom + tangentDir.Normalized() * 0.5f;
            GEditorMaster->GetPrimitiveRenderer()->DrawLine(lineFrom, tangentTo, 1.0f, Color::GRAY);
        }

        auto inwardDir = CubicBezierDoublePrime(p0, p1, p2, p3, tStep * (float)i);
        if (bShowInward)
        {
            auto inwardTo = lineFrom + inwardDir.Normalized() * 0.5f;
            GEditorMaster->GetPrimitiveRenderer()->DrawLine(lineFrom, inwardTo, 1.0f, Color::RED);
        }

        auto biNormal = tangentDir.CrossProduct(inwardDir);
        if (bShowBiNormal)
        {
            auto biNormalTo = lineFrom + biNormal.Normalized() * 0.5f;
            GEditorMaster->GetPrimitiveRenderer()->DrawLine(lineFrom, biNormalTo, 1.0f, Color::BLUE);
        }

        if (bShowNormal)
        {
            auto normalDir = biNormal.CrossProduct(tangentDir);
            auto normalTo = lineFrom + normalDir.Normalized() * 0.5f;
            GEditorMaster->GetPrimitiveRenderer()->DrawLine(lineFrom, normalTo, 1.0f, Color::GREEN);
        }
    }
}

void FBezierCurve::GenerateSweepMesh(FBezierCurveControlPointPrimitive* cp0, FBezierCurveControlPointPrimitive* cp1)
{
    //Assume SweepMesh is nullptr
    SweepMesh = new FStaticMesh();
    LOGDEBUG("Generating new mesh for curve\n");

    float dist = cp0->GetDistance(*cp1);
    int numLineSegments = (int)(dist * 2.0f);
    float tStep = 1.0f / (float)numLineSegments;
    for (int i = 0; i < numLineSegments; i++)
    {
        const auto& p0 = cp0->GetMiddlePoint().GetPosition();
        const auto& p1 = cp0->GetFrontPoint().GetPosition();
        const auto& p2 = cp1->GetBackPoint().GetPosition();
        const auto& p3 = cp1->GetMiddlePoint().GetPosition();
        auto lineFrom = CubicBezierInterp(p0, p1, p2, p3, tStep * (float)i);
        auto lineTo = CubicBezierInterp(p0, p1, p2, p3, tStep * (float)(i + 1));

        auto tangentDir = CubicBezierPrime(p0, p1, p2, p3, tStep * (float)i);
        auto inwardDir = CubicBezierDoublePrime(p0, p1, p2, p3, tStep * (float)i);
        auto biNormal = tangentDir.CrossProduct(inwardDir);
        auto normalDir = biNormal.CrossProduct(tangentDir);

		auto tangentDir2 = CubicBezierPrime(p0, p1, p2, p3, tStep * (float)(i+1));
		auto inwardDir2 = CubicBezierDoublePrime(p0, p1, p2, p3, tStep * (float)(i+1));
		auto biNormal2 = tangentDir2.CrossProduct(inwardDir2);
		auto normalDir2 = biNormal2.CrossProduct(tangentDir2);

        Quaternion localRotation = Quaternion(tangentDir.Normalized(), normalDir.Normalized(), biNormal.Normalized());
        Matrix3x4 matToParent = Matrix3x4(lineFrom, localRotation, Vector3::ONE);
		Quaternion localRotation2 = Quaternion(tangentDir2.Normalized(), normalDir2.Normalized(), biNormal2.Normalized());
		Matrix3x4 matToParent2 = Matrix3x4(lineTo, localRotation2, Vector3::ONE);

		SweepMesh->AddTriangle(matToParent * Vector3(0, 0, -0.4), matToParent * Vector3(0, 0, 0.4), matToParent2 * Vector3(0, 0, 0.4));
		SweepMesh->AddTriangle(matToParent2 * Vector3(0, 0, -0.4), matToParent * Vector3(0, 0, -0.4), matToParent2 * Vector3(0, 0, 0.4));
        //SweepMesh->AddTriangle(matToParent * Vector3(0,0,-0.4), matToParent * Vector3(0, 0.4, 0), matToParent * Vector3(0, 0, 0.4));
		//SweepMesh->AddTriangle(matToParent * Vector3(0, 0, 0.4), matToParent * Vector3(0,-0.4,0), matToParent * Vector3(0, 0, -0.4));
    }

    GEditorMaster->RegisterEntity(SweepMesh, true);
}

void FBezierCurve::Render()
{
    size_t nSegments = ControlPoints.size() - 1;
    for (auto i = 0; i < nSegments; i++)
    {
        RenderBezierSegment(ControlPoints[i], ControlPoints[i + 1]);
    }

    if (bSweep && !SweepMesh)
    {
        for (auto i = 0; i < nSegments; i++)
        {
            GenerateSweepMesh(ControlPoints[i], ControlPoints[i + 1]);
        }
    }
    else if (!bSweep && SweepMesh)
    {
        //Unchecked Sweep, delete Mesh
        delete SweepMesh;
        SweepMesh = nullptr;
        LOGDEBUG("Removed mesh for curve\n");
    }
}

void FBezierCurve::ImGuiUpdate(FInteractionSystem* interactionSystem)
{
    ImGui::Checkbox("Sweep", &bSweep);
    ImGui::Checkbox("Show Tangent", &bShowTan);
    ImGui::Checkbox("Show Inward", &bShowInward);
    ImGui::Checkbox("Show Normal", &bShowNormal);
    ImGui::Checkbox("Show BiNormal", &bShowBiNormal);
}

FBezierCurve::~FBezierCurve()
{
    delete SweepMesh;
}

float FBezierCurve::IntersectionTester::RayHitDistance(const Ray& ray)
{
    auto testSeg = [=](const Vector3& middle, const Vector3& front, const Ray& ray)
    {
        static const float DISTANCE_THRESHOLD = 0.2f;
        const Vector3& origin = middle;
        const Vector3& target = front;
        Vector3 dir = target - origin;
        Ray rayAlongSeg = Ray(origin, dir);
        Vector3 closestPoint = rayAlongSeg.ClosestPoint(ray);
        float distance = ray.Distance(closestPoint);
        if (distance > DISTANCE_THRESHOLD)
            return M_INFINITY;
        Vector3 pointOffset = closestPoint - origin;
        if (pointOffset.DotProduct(dir) < 0)
            return M_INFINITY;
        if (pointOffset.LengthSquared() > dir.LengthSquared())
            return M_INFINITY;
        return distance;
    };

    float minDist = M_INFINITY;
    size_t nSegments = Owner->ControlPoints.size() - 1;
    for (auto i = 0; i < nSegments; i++)
    {
        minDist = Min(testSeg(Owner->ControlPoints[i]->GetMiddlePoint().GetPosition(),
                              Owner->ControlPoints[i + 1]->GetMiddlePoint().GetPosition(), ray), minDist);
    }
    return minDist;
}

} /* namespace tc */
