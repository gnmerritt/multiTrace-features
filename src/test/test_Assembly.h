/**
 * test_Assembly.h
 *
 *	Unit tests for a single Assembly, to try and ensure that it behaves
 *	as expected.
 *
 *  Created on: Feb 3, 2011
 *      Author: Nathan Merritt
 *
 *  @see Assembly.h
 */

#ifndef TEST_ASSEMBLY_H_
#define TEST_ASSEMBLY_H_

#include "AssemblyIncludes.h"

Assembly_t* initializeAssembly();

// Assembly-level
bool noInput();
bool singleInput();
bool multipleInputs();
bool testInhibition();

#ifdef DEBUG_OUTPUT
	FILE *noInput_f, *singleInput_f, *multipleInputs_f, *testInhibition_f;

	void initializeAssemblyDebugFiles();
	void closeAssemblyDebugFiles();

	char* assembly_tick = "%d\t%f\t%f\t%f\t%f\t%f\n";
	char* assembly_init = "Time\tActivity\tLTCS\tSTCS\tFatigue\tregional_activation\n";
#endif

#endif /* TEST_ASSEMBLY_H_ */
