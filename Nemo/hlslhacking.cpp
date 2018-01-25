#include "hlslhacking.h"
#include "ui_hlslhacking.h"

HlslHacking::HlslHacking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HlslHacking)
{
    ui->setupUi(this);
}

HlslHacking::~HlslHacking()
{
    delete ui;
}
