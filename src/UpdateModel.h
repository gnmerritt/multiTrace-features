/*
 * UpdateModel.h
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef UPDATEMODEL_H_
#define UPDATEMODEL_H_

#include "AssemblyState.h"

class UpdateModel {
public:
	UpdateModel();
	virtual ~UpdateModel();

	virtual void tick() = 0;
};

#endif /* UPDATEMODEL_H_ */
