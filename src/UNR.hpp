/**
 * UNR.h
 *
 *      @brief Connects a certain percentage of Assemblies, ignoring distance
 *
 *      A simple UNiform Random Connection pattern, which connects
 *      a fixed percentage of Assemblies that are within one Layer of
 *      each other.
 *
 *  @date Feb 8, 2011
 *      @author Nathan Merritt
 */

#ifndef UNR_H_
#define UNR_H_

#include "ConnectionPattern.hpp"

#include <time.h>
#include <iostream>

class UNR: public ConnectionPattern {
public:
     UNR();
     virtual ~UNR();

     bool areConnected(AssemblyLocation sender, AssemblyLocation receiver);
};

#endif /* UNR_H_ */
