#include "cortexviewer.h"
#include "ui_cortexviewer.h"

CortexViewer::CortexViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CortexViewer)
{
    ui->setupUi(this);
}

CortexViewer::~CortexViewer()
{
    delete ui;
}
