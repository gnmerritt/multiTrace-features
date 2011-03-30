/*
 * UpdateModels.hpp
 *
 *  Created on: Mar 24, 2011
 *      Author: Nathan Merritt
 */

#ifndef UPDATEMODELS_HPP_
#define UPDATEMODELS_HPP_

#include "UpdateModel.hpp"
#include "SonntagUpdate.hpp"

class UpdateModels {
public:
	virtual ~UpdateModels() {
	}

	enum classes {
		SONNTAG_UPDATE,
	};

	static UpdateModel* instanceOf(int id) {
		switch (id) {
		case (SONNTAG_UPDATE):
			return new SonntagUpdate();
		default:
                        return NULL;
		}
	}

private:
	UpdateModels() {
	}

};

#endif /* UPDATEMODELS_HPP_ */
