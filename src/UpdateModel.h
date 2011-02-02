/*
 * UpdateModel.h
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef UPDATEMODEL_H_
#define UPDATEMODEL_H_

class UpdateModel {
public:
	virtual UpdateModel(AssemblyState *_state) = 0;
	virtual ~UpdateModel();

	virtual void tick() = 0;

public:
	AssemblyState *state;
};

#endif /* UPDATEMODEL_H_ */
