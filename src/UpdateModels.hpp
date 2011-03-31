/**
 * UpdateModels.hpp
 *
 *	@brief Static factory for UpdateModel derived classes
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

	/**
	 * Builds and returns an object of an unknown type that inherits
	 * from UpdateModel. Used for dynamic type control.
	 *
	 * @param id ID of UpdateModel derived class
	 * @see UpdateModels::classes
	 * @return UpdateModel derived object of the requested tyoe
	 */
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
