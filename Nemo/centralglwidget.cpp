#include "centralglwidget.h"
#include "NemoRoot.h"
#include "DumbRenderer.h"
#include "RenderTarget.h"
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
    : QOpenGLWidget(parent), bMouseInWindow(false), bIsFpsControl(false), bHideAndPinCursor(false)
{
}

bool CentralGLWidget::IsFpsControl() const
{
    return bIsFpsControl;
}

void CentralGLWidget::SetFpsControl(bool value, bool pinCursor)
{
    bIsFpsControl = value;
    bHideAndPinCursor = pinCursor;

    if (bIsFpsControl)
    {
        if (bHideAndPinCursor)
        {
            auto myCursor = cursor();
            PinnedX = myCursor.pos().x();
            PinnedY = myCursor.pos().y();
            myCursor.setShape(Qt::BlankCursor);
            setCursor(myCursor);
        }
    }
    else
    {
        auto myCursor = cursor();
        myCursor.setShape(Qt::ArrowCursor);
        setCursor(myCursor);
        bHideAndPinCursor = false;
    }
}

int CentralGLWidget::GetFrameCount() const
{
    return FrameCount;
}

void CentralGLWidget::initializeGL()
{
    MyContext = context();
    if (!GNemoRoot.InitGL(MyGetProcAddress))
        LOGFATAL("GNemoRoot.InitGL failed\n");

    FRenderTarget* thisRt = new FRenderTarget(defaultFramebufferObject());
    GNemoRoot.GetMainRenderer()->SetInitialRenderTarget(thisRt);
    if (!GNemoRoot.GetMainRenderer()->Init())
        LOGFATAL("DumbRenderer init failed\n");

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
    if (event->button() == Qt::RightButton)
    {
        event->accept();
        SetFpsControl(true, true);
        return;
    }
    QWidget::mousePressEvent(event);
}

void CentralGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        event->accept();
        SetFpsControl(false);
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void CentralGLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    LOGDEBUG("CentralGLWidget::mouseDoubleClickEvent\n");
    QWidget::mouseDoubleClickEvent(event);
}

void CentralGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (bHideAndPinCursor)
    {
        int currX, currY;
        auto myCursor = cursor();
        currX = myCursor.pos().x();
        currY = myCursor.pos().y();
        myCursor.setPos(PinnedX, PinnedY);
        setCursor(myCursor);

        if (bIsFpsControl)
        {
            CameraController.AxisEvent(PinnedX - currX, PinnedY - currY);
        }
        event->accept();
        return;
    }

    int x = event->x(), y = event->y();
    if (!bMouseInWindow)
    {
        bMouseInWindow = true;
        LastMouseX = x;
        LastMouseY = y;
    }
    /*FInputEvent inputEvt;
    auto& engineEvt = inputEvt.MouseMotion;
    engineEvt.Type = EInputEventType::MouseMotion;
    engineEvt.WindowID = engineEvt.Which = 0;
    engineEvt.x = x;
    engineEvt.y = y;
    engineEvt.xRel = x - LastMouseX;
    engineEvt.yRel = y - LastMouseY;
    GNemoRoot.StampAndQueueInputEvent(inputEvt);*/

    if (bIsFpsControl)
    {
        CameraController.AxisEvent(LastMouseX - x, LastMouseY - y);
    }

    LastMouseX = x;
    LastMouseY = y;
    event->accept();
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

void CentralGLWidget::keyPressEvent(QKeyEvent* event)
{
    LOGDEBUG("CentralGLWidget::keyPressEvent\n");
    QWidget::keyPressEvent(event);
}

void CentralGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    LOGDEBUG("CentralGLWidget::keyReleaseEvent\n");
    QWidget::keyReleaseEvent(event);
}
