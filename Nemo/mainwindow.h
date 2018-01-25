#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class HlslHacking;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionCamMinusZ_triggered();
    void on_actionCamPlusZ_triggered();
    void on_actionOrbitLeft_triggered();
    void on_actionOrbitRight_triggered();
    void on_actionFPSControl_toggled(bool isChecked);

    void updateFpsLabel();

    void on_btnShowHlslHack_clicked();

private:
    Ui::MainWindow *ui;
    HlslHacking* hlslHackingWindow;
};

#endif // MAINWINDOW_H
