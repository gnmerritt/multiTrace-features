/**
 * AssemblyState.h
 *
 *  @date Feb 2, 2011
 *      @author Nathan Merritt
 *
 *  @see Assembly.hpp
 */

#ifndef ASSEMBLYSTATE_H_
#define ASSEMBLYSTATE_H_

#include <boost/shared_ptr.hpp>

const float INITIAL_INTRA_LTCS = 0.5;

/**
 * @brief Contains the state variables for an Assembly.
 *
 *  activity -> current activation of the Assembly
 *  output -> output of the Assembly (A(t-1))
 *  fatige -> grows with increasing activation
 *
 *  Measures of the Assembly's ability to self-activate:
 *  ltcs -> long term connection strength (locked at 0)
 *  stcs -> short term connection strength
 */
class AssemblyState {
public:
     typedef boost::shared_ptr<AssemblyState> ptr;

public:
     AssemblyState() :
          activity(0), output(0), ltcs(INITIAL_INTRA_LTCS), stcs(0), fatigue(0), manual_input(0),
          regional_activation(0), lateral_inhibition(0)
          {
          }
     virtual ~AssemblyState() {
     }

     //Copy constructor
     AssemblyState(const AssemblyState &a) :
          activity(a.activity), output(a.output), ltcs(a.ltcs), stcs(a.stcs), fatigue(a.fatigue),
          regional_activation(a.regional_activation)
          {
          }
     /** multiTRACE variables */
     float activity;
     float output;
     const float ltcs; /** const because we are not modeling intra-Assembly learning */
     float stcs;
     float fatigue;

     float manual_input; // replaces input calculations for 1 tick

     /** set by Layer when it calls tick() on each assembly */
     float regional_activation;
     float lateral_inhibition;
};

#endif /* ASSEMBLYSTATE_H_ */
