#include "BezierCurve.h"

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

Vector3 CubicBezierInterp(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float t)
{
    Vector3 e = a + t * (b - a);
    Vector3 f = b + t * (c - b);
    Vector3 g = c + t * (d - c);
    return QuadraticBezierInterp(e, f, g, t);
}

FBezierCurveControlPointPrimitive::FBezierCurveControlPointPrimitive()
{
    FrontPoint.GetPosition() = Vector3(1.0f, 1.0f, 0.0f);
    MiddlePoint.GetPosition() = Vector3(0.0f, 0.0f, 0.0f);
    BackPoint.GetPosition() = Vector3(-1.0f, -1.0f, 0.0f);
    IntersectionTester.Owner = this;
}

//Begin FBezierCurveControlPointPrimitive
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
    return (rhs.MiddlePoint.GetPosition() - MiddlePoint.GetPosition()).Length();
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

FBezierCurveRenderComponentStaticData::FBezierCurveRenderComponentStaticData()
{
    UserCount++;
}

FBezierCurveRenderComponentStaticData::~FBezierCurveRenderComponentStaticData()
{
    UserCount--;
}

void FBezierCurveRenderComponentStaticData::RenderStaticInit()
{
    //StaticInit preamble
    if (bInitialized)
        return;
    bInitialized = true;

    LOGDEBUG("FBezierCurveRenderComponentStaticData::RenderStaticInit called and ran\n");

    glGenBuffers(1, Buffers);
    glGenVertexArrays(1, VertexArrays);
    glBindVertexArray(VertexArrays[0]);

    const float origin[] = {0.0f, 0.0f, 0.0f};

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, (1024) * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Shader = FGLSLProgram::CreateFromFiles("fgizmocolor.vert", "fgizmocolor.frag");
}

void FBezierCurveRenderComponentStaticData::RenderStaticDestroy()
{
    //StaticDestroy preamble
    if (!bInitialized || UserCount > 0)
        return;

    delete Shader;
    glDeleteBuffers(1, Buffers);
    glDeleteVertexArrays(1, VertexArrays);
}

int FBezierCurveRenderComponentStaticData::UserCount = 0;
bool FBezierCurveRenderComponentStaticData::bInitialized = false;
GLuint FBezierCurveRenderComponentStaticData::Buffers[1];
GLuint FBezierCurveRenderComponentStaticData::VertexArrays[1];
FGLSLProgram* FBezierCurveRenderComponentStaticData::Shader;

} /* namespace tc */
