/*
 * ConnectionPatterns.h
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
