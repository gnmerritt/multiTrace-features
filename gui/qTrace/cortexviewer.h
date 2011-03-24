#ifndef CORTEXVIEWER_H
#define CORTEXVIEWER_H

#include <QWidget>

#include "multiTrace.hpp"

namespace Ui {
    class CortexViewer;
}

class CortexViewer : public QWidget
{
    Q_OBJECT

public:
    explicit CortexViewer(QWidget *parent = 0);
    ~CortexViewer();

    void setCortex(Cortex::ptr);

private:
    Cortex::ptr thisCortex;

private:
    Ui::CortexViewer *ui;

private slots:
    void on_tickButton_clicked();
};

#endif // CORTEXVIEWER_H
