/**
 * ConnectionPatterns.h
 *
 *	@brief Static factory for ConnectionPattern derived classes
 *
 *  Created on: Mar 24, 2011
 *      Author: Nathan Merritt
 */

#ifndef CONNECTIONPATTERNS_H_
#define CONNECTIONPATTERNS_H_

#include "ConnectionPattern.hpp"
#include "UNR.hpp"

class ConnectionPatterns {
public:
	virtual ~ConnectionPatterns() {
	}

	enum classes {
		UNR_PATTERN,
	};

	/**
	 * Builds and returns an object of an unknown type that inherits
	 * from ConnectionPattern. Used for dynamic type control.
	 *
	 * @param id ID of ConnectionPattern derived class
	 * @see ConnectionPatterns::classes
	 * @return ConnectionPattern derived object of the requested tyoe
	 */
	static ConnectionPattern instanceOf(int id) {
		switch (id) {
		case (UNR_PATTERN): {
			UNR p;
			return p;
		}
		default: {
			ConnectionPattern c;
			return c;
		}
		}
	}

private:
	ConnectionPatterns() {
	}
};

#endif /* CONNECTIONPATTERNS_H_ */
