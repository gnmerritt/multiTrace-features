#include "cortexthread.hpp"

CortexThread::CortexThread(QObject *parent) :
	QObject(parent)
{
}


/**
 * @brief ticks the internal Cortex object. Nothing fancy.
 */
void CortexThread::tick() {
	ourCortex.tick();
	emit redraw();
}
