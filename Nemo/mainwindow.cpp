#include "mainwindow.h"
#include "centralglwidget.h"
#include "ui_mainwindow.h"
#include "scenemodel.h"
#include "NemoRoot.h"
#include "NodeFactory.h"
#include "NemoSceneNode.h"
#include "Serialization.h"
#include "hlslhacking.h"
#include <ResourceManager.h>
#include <Scene.h>
#include <QPushButton>
#include <QFileDialog>
#include <QTimer>

#include <yaml-cpp/yaml.h>

using namespace tc;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    hlslHackingWindow = new HlslHacking();
    hlslHackingWindow->show();

    connect(ui->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);

    if (!GNemoRoot.Init())
        ui->statusbar->setStatusTip("We have a problem");

    GResourceManager.Init();

    auto* model = new FSceneModel(this, GNemoRoot.GetMainScene()->GetRootNode());
    ui->sceneTree->setModel(model);

    connect(ui->openGLWidget, &CentralGLWidget::frameCountChanged, this, &MainWindow::updateFpsLabel);
}

MainWindow::~MainWindow()
{
    GNemoRoot.Shutdown();
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Open Scene", QString(), "YAML files (*.yaml);;Text files (*.txt)");
    if (fileName.isEmpty())
        return;
    LOGDEBUG("[MainWindow] %s\n", fileName.toStdString().c_str());
    YAML::Node sceneFile = YAML::LoadFile(fileName.toStdString());
    auto rootKind = sceneFile["Kind"].as<string>();
    LOGINFO("Root node is of type %s\n", rootKind.c_str());

    FYamlDeserializer deserializer = FYamlDeserializer(sceneFile);
    auto* rootNode = FNodeFactory::NewSceneNode(rootKind);
    rootNode->Reconstruct(&deserializer);

    GNemoRoot.GetMainScene()->GetRootNode()->AddChild(rootNode);
    rootNode->ForEachNodeDeep([](FSceneNode* node){
        auto* nemoNode = dynamic_cast<FNemoSceneNode*>(node);
        if (nemoNode)
            nemoNode->PostReconstruct();
    });
    ui->sceneTree->reset();
}

void MainWindow::on_actionCamMinusZ_triggered()
{
    auto camNode = GNemoRoot.GetMainScene()->GetActiveCameraNode();
    camNode->Translate(0.0f, 0.0f, -0.1f);
}

void MainWindow::on_actionCamPlusZ_triggered()
{
    auto camNode = GNemoRoot.GetMainScene()->GetActiveCameraNode();
    camNode->Translate(0.0f, 0.0f, 0.1f);
}

void MainWindow::on_actionOrbitLeft_triggered()
{
    auto camPivot = GNemoRoot.GetMainScene()->GetActiveCameraNode()->GetParent();
    camPivot->Yaw(-5.0f);
}

void MainWindow::on_actionOrbitRight_triggered()
{
    auto camPivot = GNemoRoot.GetMainScene()->GetActiveCameraNode()->GetParent();
    camPivot->Yaw(5.0f);
}

void MainWindow::on_actionFPSControl_toggled(bool isChecked)
{
    ui->openGLWidget->SetFpsControl(isChecked);
}

void MainWindow::updateFpsLabel()
{
    auto totalTime = GNemoRoot.GetRootTimer()->GetTotalTime();
    auto timeStr = QString::number(totalTime);
    auto fcount = ui->openGLWidget->GetFrameCount();
    ui->fpsLabel->setText(QString::number(fcount) + " t=" + timeStr);
}

void MainWindow::on_btnShowHlslHack_clicked()
{
    hlslHackingWindow->show();
}
