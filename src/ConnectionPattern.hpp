/**
 * ConnectionPattern.h
 *
 * Base class that other connection patterns inherit from. Provides a method
 * that determines whether or not two Assemblies are connected based on pointers
 * to their state and their AssemblyLocation in space
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt

 * @see AssemblyLocation
 */

#ifndef CONNECTIONPATTERN_H_
#define CONNECTIONPATTERN_H_

class AssemblyLocation;

class ConnectionPattern {
public:
	ConnectionPattern() {
	}
	virtual ~ConnectionPattern() {
	}

	virtual bool areConnected(AssemblyLocation sender, AssemblyLocation receiver) = 0;
};

/**
 * Describes an Assembly in 3-space (Row,Col,Layer)
 */
class AssemblyLocation {
public:
	AssemblyLocation(int _row, int _col, int _layer) :
		row(_row), column(_col), layer(_layer) {

	}
	virtual ~AssemblyLocation() {
	}

public:
	int row, column, layer;
};

#endif /* CONNECTIONPATTERN_H_ */
