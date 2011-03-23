/*
 * UNR.cpp
 *
 *  Created on: Feb 8, 2011
 *      Author: Nathan Merritt
 */

#include "UNR.hpp"

const float PERCENT_CONNECTED = 0.85;
const int MAX_CONNECTION_Z = 1;

UNR::UNR() {
	srand( time(NULL) );
}

UNR::~UNR() {
	// TODO Auto-generated destructor stub
}

/**
 * Determine whether these two Assemblies are connected, using a uniform
 * random distribution (really: checking if a random float [0,1] is larger
 * than a pre-determined percentage. Only allows connections between neighboring
 * Layers (by AssemblyLocation.layer)
 *
 * @param sender
 * @param receiver
 * @return true if the two Assemblies are connected;
 */
bool UNR::areConnected(AssemblyLocation sender, AssemblyLocation receiver) {
	// first check how far apart the locations (Layer only)
	int layerDifference = abs(sender.layer - receiver.layer);

	if (layerDifference > MAX_CONNECTION_Z) {
		return false;
	}

	float random = rand()/double(RAND_MAX);

	return (random < PERCENT_CONNECTED);
}
