/**
 * test_Assembly.h
 *
 *	Unit tests for a single Assembly, to try and ensure that it behaves
 *	as expected.
 *
 *  @date Feb 3, 2011
 *      @author Nathan Merritt
 *
 *  @see Assembly.h
 */

#ifndef TEST_ASSEMBLY_H_
#define TEST_ASSEMBLY_H_

#include "Tests.h"

#include "AssemblyIncludes.hpp"

#define DEBUG_ASSEMBLY_OUTPUT

Assembly* initializeAssembly();

// Assembly-level
void noInput();
void singleInput();
void multipleInputs();
void testInhibition();

#endif /* TEST_ASSEMBLY_H_ */
