#include "centralglwidget.h"
#include "NemoRoot.h"
#include "DumbRenderer.h"
#include <Scene.h>
#include <QOpenGLContext>
#include <QMouseEvent>

using namespace tc;

static QOpenGLContext* MyContext;
void *MyGetProcAddress(const char * name)
{
    return (void*)(MyContext->getProcAddress(name));
}

CentralGLWidget::CentralGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), bMouseInWindow(false), bIsFpsControl(false)
{
}

bool CentralGLWidget::IsFpsControl() const
{
    return bIsFpsControl;
}

void CentralGLWidget::SetFpsControl(bool value)
{
    bIsFpsControl = value;
}

int CentralGLWidget::GetFrameCount() const
{
    return FrameCount;
}

void CentralGLWidget::initializeGL()
{
    MyContext = context();
    GNemoRoot.InitGL(MyGetProcAddress);

    setMouseTracking(true);
    CameraController.SetCameraNode(GNemoRoot.GetMainScene()->GetActiveCameraNode());
    CameraController.SetAxisScaleFactor(0.05f);

    auto* verStr = glGetString(GL_VERSION);
    printf("%s\n", verStr);

    UpdateTimer = new QTimer(this);
    connect(UpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
    UpdateTimer->start();

    FrameCount = 0;
}

void CentralGLWidget::resizeGL(int w, int h)
{
    FRenderViewInfo renderViewInfo;
    renderViewInfo.Width = w;
    renderViewInfo.Height = h;
    renderViewInfo.SetAspectRatioFromWidthHeight();
    GNemoRoot.GetMainRenderer()->SetRenderViewInfo(renderViewInfo);
}

void CentralGLWidget::paintGL()
{
    GNemoRoot.RenderScene();
    FrameCount++;
    emit frameCountChanged();
}

void CentralGLWidget::mousePressEvent(QMouseEvent *event)
{
    LOGDEBUG("CentralGLWidget::mousePressEvent\n");
    QWidget::mousePressEvent(event);
}

void CentralGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    LOGDEBUG("CentralGLWidget::mouseReleaseEvent\n");
    QWidget::mouseReleaseEvent(event);
}

void CentralGLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    LOGDEBUG("CentralGLWidget::mouseDoubleClickEvent\n");
    QWidget::mouseDoubleClickEvent(event);
}

void CentralGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x(), y = event->y();
    if (!bMouseInWindow)
    {
        bMouseInWindow = true;
        LastMouseX = x;
        LastMouseY = y;
    }
    FInputEvent inputEvt;
    auto& engineEvt = inputEvt.MouseMotion;
    engineEvt.Type = EInputEventType::MouseMotion;
    engineEvt.WindowID = engineEvt.Which = 0;
    engineEvt.x = x;
    engineEvt.y = y;
    engineEvt.xRel = x - LastMouseX;
    engineEvt.yRel = y - LastMouseY;
    GNemoRoot.StampAndQueueInputEvent(inputEvt);
    event->accept();

    if (bIsFpsControl)
    {
        CameraController.AxisEvent(LastMouseX - x, LastMouseY - y);
    }

    LastMouseX = x;
    LastMouseY = y;
}

void CentralGLWidget::enterEvent(QEvent *event)
{
    LOGDEBUG("CentralGLWidget::enterEvent\n");
    QWidget::enterEvent(event);
}

void CentralGLWidget::leaveEvent(QEvent *event)
{
    LOGDEBUG("CentralGLWidget::leaveEvent\n");
    bMouseInWindow = false;
    event->accept();
}
