#include "cortexthread.hpp"

CortexThread::CortexThread(QObject *parent) :
	QObject(parent)
{
}

void CortexThread::setCortex(Cortex::ptr _cortex){
	ourCortex = _cortex;

	Layer::vector *layers = ourCortex->getLayers();

	Layer::vector::iterator layer;

	// launch the LayerViewer widgets
	for (layer = layers->begin(); layer != layers->end(); ++layer) {
		LayerViewer *lv = new LayerViewer();

		// sets new LayerViewer to receive ticks from this controller
		lv->connect(this,
					SIGNAL(redraw()),
					SLOT(update()));

		lv->setLayer(*layer);
		lv->show();
	}
}

/**
 * @brief ticks the internal Cortex object. Nothing fancy.
 */
void CortexThread::tick() {
	ourCortex->tick();
	emit redraw();
}
