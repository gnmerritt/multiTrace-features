/**
 * ConnectionPattern.hpp
 *
 *  @date Feb 2, 2011
 *  @author Nathan Merritt
 *  @see AssemblyLocation
 */

#ifndef CONNECTIONPATTERN_H_
#define CONNECTIONPATTERN_H_

#include <boost/shared_ptr.hpp>

/**
 * @brief Describes an Assembly in 3-space (Row,Col,Layer)
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

/** @brief Control structure for connecting two Layer s
 *
 * Base class that other connection patterns inherit from. Provides a method
 * that determines whether or not two Assemblies are connected based on pointers
 * to their state and their AssemblyLocation in space
 */
class ConnectionPattern {
public:
	typedef boost::shared_ptr<ConnectionPattern> ptr;

public:
	ConnectionPattern() {
	}
	virtual ~ConnectionPattern() {
	}

	/**
	 * Determines whether two Assembly s should be connected, given their AssemblyLocation
	 * inside this Cortex. Not all derived classes will use this data, but it is provided
	 * by default.
	 *
	 * @param sender Sending Assembly row, col, layer
	 * @param receiver receiving Assembly row ,col, layer
	 */
	virtual bool areConnected(AssemblyLocation sender, AssemblyLocation receiver) = 0;
};

#endif /* CONNECTIONPATTERN_H_ */
