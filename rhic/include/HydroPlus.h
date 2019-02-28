//
//  HydroPlus.h
//  
//
//  Created by Lipei Du on 10/17/18.
//

// Equation indices from PRD 98 (2018) 036006

#ifndef HydroPlus_h
#define HydroPlus_h

#include "../include/DynamicalVariables.h"
#include "../include/LatticeParameters.h"
#include "../include/InitialConditionParameters.h"
#include "../include/HydroParameters.h"

PRECISION xi(PRECISION T, PRECISION muB);

PRECISION correlationLength(PRECISION T, PRECISION muB);

PRECISION equilibriumPhiQ(PRECISION e, PRECISION rhob, PRECISION T, PRECISION muB, PRECISION s, PRECISION Q);

PRECISION relaxationCoefficientPhi(PRECISION rhob, PRECISION s, PRECISION T, PRECISION corrL2);

PRECISION relaxationCoefficientPhiQ(PRECISION gammaPhi, PRECISION corrL2, PRECISION Q);

PRECISION dLnxidE(PRECISION e0, PRECISION rhob0);

PRECISION dLnxidN(PRECISION e0, PRECISION rhob0);

void getCorrelationLengthTable();

void setInitialConditionSlowModes(void * latticeParams, void * hydroParams);

void getPressurePlusFromSlowModes(PRECISION * const __restrict__ pPlus, const PRECISION * const __restrict__ equiPhiQ, const PRECISION * const __restrict__ PhiQ, PRECISION e, PRECISION rhob, PRECISION p, PRECISION T, PRECISION alphaB, PRECISION s);

#endif /* HydroPlus_h */
