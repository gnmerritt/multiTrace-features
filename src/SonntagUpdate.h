/*
 * SonntagUpdate.h
 *
 *	Updates the Assembly state according to the equations derived in
 *
 *	"Sonntag M (1991) Learning sequences in an associative network:
 *	A step towards cognitive structure." (pgs 77, 94)
 *
 *	in a multiTRACE unit:
 *	O(t+1) = A(t)
 *	A(t+1) = A(t) + ΔA,	A(0) = 0.01
 *	L(t+1) = L(t) + ΔL,	L(0) = 0
 *	S(t+1) = S(t) + ΔS, S(0) = 0.01
 *	F(t+1) = F(t) + ΔF, F(0) = 0.01
 *
 *	where ΔA = ΔAgrowth - ΔAdecline,
 *	GROWTH derivation:
 *		ΔAgrowth = A (1 - A) V, where V can be thought of as sensitivity
 *		V(t) = ((L + S) (1 - F) / v, where v is a normalization factor
 *
 *		Incorporating input term I, this gives us:
 *		ΔAgrowth = (A + I (1 - A)) (1 - A) V
 *
 *	DECLINE derivation:
 *		ΔAdecline = (A^thetaL + A (1 - A)^thetaC) (1 - V)
 *
 *	TOGETHER:
 *		ΔA = (A + I (1 - A)) (1 - A) V - (A^thetaL + A (1 - A)^thetaC) (1 - V)
 *
 *	Delta Fatigue/STCS equations similar, differ only by parameter:
 *		ΔS = sigmaG A (1 - S)^2 - sigmaD*S
 *		ΔF = thetaG A (1 - F)^2 - thetaD*F
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef SONNTAGUPDATE_H_
#define SONNTAGUPDATE_H_

#include "UpdateModel.h"

class SonntagUpdate: public UpdateModel {
public:
	SonntagUpdate() {
	}
	virtual ~SonntagUpdate() {
	}

	void tick(AssemblyState *state, ConnectionVector *input);

private:
	float calculateInput();
	float calculateDeltaActivity();
	float calculateV();
	float calculateDeltaLTCS();
	float calculateDeltaSTCS();
	float calculateDeltaFatigue();
};

#endif /* SONNTAGUPDATE_H_ */
