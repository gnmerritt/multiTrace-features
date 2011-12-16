/**
 * SonntagUpdate.h
 *
 *      @brief Updates the AssemblyState according to the equations derived in (Sonntag 1991)
 *
 *      "Sonntag M (1991) Learning sequences in an associative network:
 *      A step towards cognitive structure." (pgs 77, 94)
 *
 *      in a multiTRACE unit:
 *      O(t+1) = A(t)
 *      A(t+1) = A(t) + ŒîA,    A(0) = 0.01
 *      L(t+1) = L(t) + ŒîL,    L(0) = 0
 *      S(t+1) = S(t) + ŒîS, S(0) = 0.01
 *      F(t+1) = F(t) + ŒîF, F(0) = 0.01
 *
 *      where ŒîA = ŒîAgrowth - ŒîAdecline,
 *      GROWTH derivation:
 *              ŒîAgrowth = A (1 - A) V, where V can be thought of as sensitivity
 *              V(t) = ((L + S) (1 - F) / v, where v is a normalization factor
 *
 *              Incorporating input term I, this gives us:
 *              ŒîAgrowth = (A + I (1 - A)) (1 - A) V
 *
 *      DECLINE derivation:
 *              ŒîAdecline = (A^thetaL + A (1 - A)^thetaC) (1 - V)
 *
 *      TOGETHER:
 *              ŒîA = (A + I (1 - A)) (1 - A) V - (A^thetaL + A (1 - A)^thetaC) (1 - V)
 *
 *      Delta Fatigue/STCS equations similar, differ only by parameter:
 *      NOTE: changed both (1-S) and (1-F) to linear terms (were squared) after (Forbell & Chown 2000)
 *              ŒîS = sigmaG A (1 - S) - sigmaD*S
 *              ŒîF = thetaG A (1 - F) - thetaD*F
 *
 *  @date Feb 2, 2011
 *  @author Nathan Merritt
 */

#ifndef SONNTAGUPDATE_H_
#define SONNTAGUPDATE_H_

#include "UpdateModel.hpp"

class SonntagUpdate: public UpdateModel {
public:
     /// parameters, and where they are used
     enum parameters {
          NORM_SENSITIVITY, // calculateV()
          DECAY_COMPETITION, // exponential decay terms, ΔActivity
          DECAY_LOSS,
          ACTIVATION_DAMPENING, // recurrent activation dampening
          PHI_POS, // input resistance, calculateInput()
          PHI_NEG,
          REGIONAL_INHIBITION,
          LATERAL_INHIBITION,
          EXTERNAL_DAMPENING,
          DELTA_LTCS, // used in ΔLTCS
          SIGMA_GROWTH, // exponential decay terms for ΔSTCS
          SIGMA_DECLINE,
          STCS_GAIN,
          FATIGUE_GROWTH, // exponential decay terms for ΔFatigue
          FATIGUE_DECLINE,
          PARAMETER_COUNT
     };

public:
     SonntagUpdate();
     virtual ~SonntagUpdate() {
     }

     void tick(AssemblyState::ptr state, Connection::vector *input);
     void setParameters(const float newParams[]);

private:
     float parameters[PARAMETER_COUNT];

     float calculateInput();
     float calculateDeltaActivity();
     float calculateV();
     float calculateDeltaSTCS();
     float calculateDeltaFatigue();
};

#endif /* SONNTAGUPDATE_H_ */
