/*
 * SonntagUpdate.h
 *
 *	Updates the Assembly state according to the equations derived in
 *
 *	"Sonntag M (1991) Learning sequences in an associative network:
 *	A step towards cognitive structure." (pgs 77, 94)
 *
 *	in a TRACE unit:
 *	A(t+1) = A(t) + ^A,	A(0) = 0.01
 *	L(t+1) = L(t) + ^L,	L(0) = 0
 *	S(t+1) = S(t) + ^S, S(0) = 0.01
 *	F(t+1) = F(t) + ^F, F(0) = 0.01
 *
 *	where ^A = ^Agrowth - ^Adecline,
 *	GROWTH derivation:
 *		^Agrowth = A (1 - A) V, where V can be thought of as sensitivity
 *		V(t) = ((L + S) (1 - F) / v, where v is a normalization factor
 *
 *		Incorporating input term I, this gives us:
 *		^Agrowth = (A + I (1 - A)) (1 - A) V
 *
 *	DECLINE derivation:
 *		^Adecline = (A^thetaL + A (1 - A)^thetaC) (1 - V)
 *
 *	TOGETHER:
 *		^A = (A + I (1 - A)) (1 - A) V - (A^thetaL + A (1 - A)^thetaC) (1 - V)
 *
 *	Delta Fatigue/STCS equations similar, differ only by parameter:
 *		^F = thetaG A (1 - F)^2 - thetaDF
 *		^S = sigmaG A (1 - S)^2 - sigmaDS
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef SONNTAGUPDATE_H_
#define SONNTAGUPDATE_H_

#include "UpdateModel.h"

class SonntagUpdate : public UpdateModel {
public:
	SonntagUpdate();
	virtual ~SonntagUpdate();

	void tick(AssemblyState *state, ConnectionVector *input);

private:
	AssemblyState *currentState;
	ConnectionVector *currentInput;

	float calculateInput();
	float calculateDeltaActivity();
	float calculateDeltaLTCS();
	float calculateDeltaSTCS();
	float calculateDeltaFatigue();
};

#endif /* SONNTAGUPDATE_H_ */
