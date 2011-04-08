#include "cortexviewer.h"
#include "ui_cortexviewer.h"

const int LAYER_IMG_WIDTH = 200;
const int LAYER_IMG_HEIGHT = 200;

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

void CortexViewer::update()
{
    QImage img (LAYER_IMG_WIDTH, LAYER_IMG_HEIGHT, QImage::Format_RGB32);

    Layer::vector *layers = thisCortex->getLayers();
    Layer::ptr top = layers->front();

    // pixels per Assembly, how big each Assembly's output is drawn
    int ppa_cols = LAYER_IMG_WIDTH / top->cols;
    int ppa_rows = LAYER_IMG_HEIGHT / top->rows;

    Layer::LayerOutput* layerOutput = top->getOutputBlock();

    for (int col = 0; col < LAYER_IMG_WIDTH; ++col) {
        int assemblyCol = col / ppa_cols;

        for (int row = 0; row < LAYER_IMG_HEIGHT; ++row) {
            int assemblyRow = row / ppa_rows;

            float assemblyOutput = layerOutput->at(assemblyRow).at(assemblyCol);

            QColor c;
            c.setRgbF(assemblyOutput, assemblyOutput, assemblyOutput);

            img.setPixel(col, row, c.rgb());
        }
    }
    QPixmap pix;
    pix.convertFromImage(img);
    ui->layerImg->setFixedSize(LAYER_IMG_WIDTH, LAYER_IMG_HEIGHT);
    ui->layerImg->setPixmap(pix);
    ui->layerImg->repaint();
}

void CortexViewer::setCortex(Cortex::ptr newCortex) {
    thisCortex = newCortex;
}

void CortexViewer::on_tickButton_clicked()
{
    thisCortex->tick();
    update();
}
