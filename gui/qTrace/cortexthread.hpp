#ifndef CORTEXTHREAD_HPP
#define CORTEXTHREAD_HPP

#include <QObject>

#include "QtUtility.hpp"

#include "multiTrace.hpp"

class CortexThread : public QObject
{
	Q_OBJECT
public:
	explicit CortexThread(QObject *parent = 0);

private:
	Cortex ourCortex;

signals:
	void redraw();

public slots:
	void tick();

};

#endif // CORTEXTHREAD_HPP
