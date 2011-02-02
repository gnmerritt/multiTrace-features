/*
 * UpdateModel.h
 *
 *	Abstract class responsible for updating the state variables of an
 *	assembly.
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef UPDATEMODEL_H_
#define UPDATEMODEL_H_

#include "AssemblyState.h"
#include "Connection.h"

class UpdateModel {
public:
	UpdateModel();
	virtual ~UpdateModel();

	virtual void tick(AssemblyState *state, ConnectionVector *input) = 0;
};

#endif /* UPDATEMODEL_H_ */
