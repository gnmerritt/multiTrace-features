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

#include <stdio.h>
#include <iostream>

#define DEBUG_OUTPUT

Assembly_t* initializeAssembly();

bool noInput();
bool singleInput();
bool multipleInputs();
bool testInhibition();

#ifdef DEBUG_OUTPUT
	FILE *noInput_f, *singleInput_f, *multipleInputs_f, *testInhibition_f;

	void initializeDebugFiles();
	void closeDebugFiles();
#endif

#endif /* TEST_ASSEMBLY_H_ */
