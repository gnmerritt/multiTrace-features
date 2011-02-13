/*
 * test_Layer.h
 *
 *  Created on: Feb 10, 2011
 *      Author: Nathan Merritt
 */

#ifndef TEST_LAYER_H_
#define TEST_LAYER_H_

#include "AssemblyIncludes.h"

// Layer-level
bool noInputLayer1_1();
bool noInputLayer30_30();

#ifdef DEBUG_OUTPUT
	FILE *noInputLayer1_1_f, *noInputLayer30_30_f;

	initializeLayerDebugFiles();
	closeLayerDebugFiles();

	char* layer_init;
	char* layer_tick;
#endif

#endif /* TEST_LAYER_H_ */
