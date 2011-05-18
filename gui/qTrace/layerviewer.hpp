#ifndef LAYERVIEWER_HPP
#define LAYERVIEWER_HPP

#include <vector>
#include <QWidget>

#include "multiTrace.hpp"

namespace Ui {
	class LayerViewer;
}

class LayerViewer : public QWidget
{
	Q_OBJECT

public:
	typedef std::vector<LayerViewer*> vector;

public:
	explicit LayerViewer(QWidget *parent = 0);
	~LayerViewer();

	void setLayer(Layer::ptr _thisLayer);

private:
	void stimulateRandomAssemblies();

private:
	Ui::LayerViewer *ui;
	Layer::ptr thisLayer;
	int thisLayer_id;

	int windowSizeX, windowSizeY;

	int inputRow, inputCol, inputLayer;
	float inputStrength;
	float stimulateRandom;

public slots:
	void update();

private slots:
	void on_pushButton_clicked();
	void on_stimulateRandom_valueChanged(double );
	void on_inputCol_valueChanged(int );
	void on_inputRow_valueChanged(int );
	void on_inputStrength_valueChanged(double );
	void on_addInput_clicked();
};

#endif // LAYERVIEWER_HPP
