/*
 * EquationOfState.h
 *
 *  Created on: Oct 22, 2015
 *      Author: bazow
 */

#ifndef EQUATIONOFSTATE_H_
#define EQUATIONOFSTATE_H_

#include "../include/DynamicalVariables.h"

#define CONFORMAL_EOS

// ideal gas of massless quarks and gluons
//#define EOS_FACTOR 15.6269 // Nc=3, Nf=3
#define EOS_FACTOR 13.8997 // Nc=3, Nf=2.5


void getEquationOfStateTable();//Lipei

void getPrimaryVariablesCombo(PRECISION e, PRECISION rhob, PRECISION * const __restrict__ PrimaryVariables);

PRECISION InferredPrimaryVariable(PRECISION e, PRECISION rhob, PRECISION e_start, PRECISION d_e, int nrhob, PRECISION d_rhob, int index_start, const PRECISION * const __restrict__ EOS_Variable);

// EoS with baryon

PRECISION baryonDiffusionConstant(PRECISION T, PRECISION muB);

PRECISION equilibriumPressure(PRECISION e, PRECISION rhob);

PRECISION effectiveTemperature(PRECISION e, PRECISION rhob);

PRECISION chemicalPotentialOverT(PRECISION e, PRECISION rhob);

PRECISION speedOfSoundSquared(PRECISION e, PRECISION rhob);

PRECISION dPdRhob(PRECISION e, PRECISION rhob);

PRECISION dPdE(PRECISION e, PRECISION rhob);

PRECISION equilibriumEntropy(PRECISION e, PRECISION rhob, PRECISION p, PRECISION T, PRECISION alphaB);

// Wuppertal-Budapest EoS without baryon

PRECISION equilibriumPressureWB(PRECISION e);

PRECISION effectiveTemperatureWB(PRECISION e);

PRECISION equilibriumEnergyDensityWB(PRECISION T);

#endif /* EQUATIONOFSTATE_H_ */
