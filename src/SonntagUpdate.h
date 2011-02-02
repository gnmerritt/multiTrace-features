/*
 * SonntagUpdate.h
 *
 *	Updates the Assembly state according to the equations derived in
 *
 *	"Sonntag M (1991) Learning sequences in an associative network:
 *	A step towards cognitive structure." (pgs 77, 94)
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef SONNTAGUPDATE_H_
#define SONNTAGUPDATE_H_

#include "UpdateModel.h"

class SonntagUpdate : public UpdateModel {
public:
	SonntagUpdate();
	virtual ~SonntagUpdate();

	void tick();
};

#endif /* SONNTAGUPDATE_H_ */
