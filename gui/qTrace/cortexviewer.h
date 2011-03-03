#ifndef CORTEXVIEWER_H
#define CORTEXVIEWER_H

#include <QWidget>

namespace Ui {
    class CortexViewer;
}

class CortexViewer : public QWidget
{
    Q_OBJECT

public:
    explicit CortexViewer(QWidget *parent = 0);
    ~CortexViewer();

private:
    Ui::CortexViewer *ui;
};

#endif // CORTEXVIEWER_H
