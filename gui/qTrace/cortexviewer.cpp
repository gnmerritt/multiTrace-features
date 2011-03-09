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

void CortexViewer::setCortex(Cortex<UNR, SonntagUpdate, NoLearning>::ptr newCortex) {
    thisCortex = newCortex;
}

void CortexViewer::on_tickButton_clicked()
{
    thisCortex->tick();
}
