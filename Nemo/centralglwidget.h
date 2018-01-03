#pragma once
#include "FirstPersonCameraController.h"
#include <QOpenGLWidget>
#include <QTimer>

class CentralGLWidget : public QOpenGLWidget
{
    Q_OBJECT

    int LastMouseX, LastMouseY;
    bool bMouseInWindow, bIsFpsControl;

    tc::FFirstPersonCameraController CameraController;

    QTimer* UpdateTimer;

    int FrameCount;

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

public:
    explicit CentralGLWidget(QWidget *parent = nullptr);

    bool IsFpsControl() const;
    void SetFpsControl(bool value);

    int GetFrameCount() const;

signals:
    void frameCountChanged();

public slots:
};
