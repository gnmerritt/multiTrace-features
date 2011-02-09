/**
 * UNR.h
 *
 *	A simple UNiform Random Connection pattern, which connects
 *	a fixed percentage of Assemblies that are within one Layer of
 *	each other.
 *
 *  Created on: Feb 8, 2011
 *      Author: Nathan Merritt
 */

#ifndef UNR_H_
#define UNR_H_

#include "ConnectionPattern.h"

#include <time.h>
#include <iostream>

class UNR: public ConnectionPattern {
public:
	UNR();
	virtual ~UNR();

	bool areConnected(AssemblyLocation sender, AssemblyLocation receiver);
};

#endif /* UNR_H_ */
