#include "NodeGraph.h"

#include <UsingStl.h>

#include <nanovg.h>

namespace tc
{

struct NGCanvasPrivateData
{
    set<NGBaseNode*> Nodes; //Need better data structure later
};

NGCanvas::NGCanvas()
{
    Pd = new NGCanvasPrivateData();
}

NGCanvas::~NGCanvas()
{
    delete Pd;
}

void NGCanvas::Init()
{
}

void NGCanvas::Shutdown()
{
}

void NGCanvas::Paint(NGView* view)
{
    for (auto* node : Pd->Nodes)
    {
        //If node in scene
        //TODO: implement clipping
        node->Paint(view);
    }
}

void NGCanvas::RegisterNode(NGBaseNode* node)
{
    Pd->Nodes.insert(node);
}

void NGCanvas::UnregisterNode(NGBaseNode* node)
{
    Pd->Nodes.erase(node);
}

NGBaseNode::NGBaseNode(NGCanvas* canvas) : Canvas(canvas)
{
    Canvas->RegisterNode(this);
}

NGBaseNode::~NGBaseNode()
{
    Canvas->UnregisterNode(this);
}

const char* NGBaseNode::GetName() const
{
    static const char* name = "Base Node";
    return name;
}

float NGBaseNode::GetWidth() const
{
    return 100.0f;
}

float NGBaseNode::GetHeight() const
{
    return 100.0f;
}

void NGBaseNode::Paint(NGView* view)
{
    NVGcontext* vg = view->GetNVGContext();

    auto viewPos = view->TransformVector(GetPosition());
    auto screenPos = view->GetPositionOnScreen() + viewPos;
    float x = screenPos.x_, y = screenPos.y_,
          w = GetWidth() * view->GetScale(), h = GetHeight() * view->GetScale();
    float cornerRadius = 3.0f;

    NVGpaint shadowPaint, headerPaint;

    //Background
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, w, h, cornerRadius);
    nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
    nvgFill(vg);

    //Shadow
    shadowPaint = nvgBoxGradient(vg, x, y + 2, w, h, cornerRadius * 2, 10, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
    nvgBeginPath(vg);
    nvgRect(vg, x - 10, y - 10, w + 20, h + 30);
    nvgRoundedRect(vg, x, y, w, h, cornerRadius);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, shadowPaint);
    nvgFill(vg);

    //Header
    headerPaint = nvgLinearGradient(vg, x, y, x, y + 15, nvgRGBA(255, 255, 255, 8), nvgRGBA(0, 0, 0, 16));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x + 1, y + 1, w - 2, 30, cornerRadius - 1);
    nvgFillPaint(vg, headerPaint);
    nvgFill(vg);
    nvgBeginPath(vg);
    nvgMoveTo(vg, x + 0.5f, y + 0.5f + 30);
    nvgLineTo(vg, x + 0.5f + w - 1, y + 0.5f + 30);
    nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 32));
    nvgStroke(vg);

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

    nvgFontBlur(vg, 2);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 128));
    nvgText(vg, x + w / 2, y + 16 + 1, GetName(), nullptr);

    nvgFontBlur(vg, 0);
    nvgFillColor(vg, nvgRGBA(220, 220, 220, 160));
    nvgText(vg, x + w / 2, y + 16, GetName(), nullptr);
}

} /* namespace tc */
