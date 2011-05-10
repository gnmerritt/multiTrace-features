#ifndef QTUTILITY_HPP
#define QTUTILITY_HPP

#include <QtCore>

/**
  * Convenience class so we don't have to explicity subclass QThread
  *
  */
class QThreadEx : public QThread
{
protected:
	void run() { exec(); }
};


#endif // QTUTILITY_HPP
