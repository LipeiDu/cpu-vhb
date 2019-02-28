//
//  HydroPlus.cpp
//  
//
//  Created by Lipei Du on 10/17/18.
//

// Equation indices from PRD 98 (2018) 036006

#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <iostream>//Lipei
#include <istream>//Lipei
#include <fstream>//Lipei
#include <stdio.h>//Lipei
#include <cassert>//Lipei
#include <string>//Lipei
#include <iomanip>//by Lipei
using namespace std;//Lipei

#include "../include/InitialConditions.h"
#include "../include/DynamicalVariables.h"
#include "../include/LatticeParameters.h"
#include "../include/InitialConditionParameters.h"
#include "../include/HydroParameters.h"
#include "../include/EquationOfState.h"
#include "../include/HydroPlus.h"

#define dQvec 10.0 // difference between Q vectors of slow modes
#define Q0 0.0
#define HBARC 0.197326938

/**************************************************************************************************************************************************/
/* Conductivity, heat capacity, correlation length and relaxation coefficients of different slow modes, and their derivatives
/**************************************************************************************************************************************************/

// for testing
#define Cr 1.0 // LambdaT = Cr * T^2
#define xi0 1.0
#define xi02 1.0 // correlation length squared
#define sigmae 50.0
#define sigman 10.0
#define ec 30.0
#define rhobc 30.0
#define Tc 200.0 // to be fixed

// heat conductivity
PRECISION lambdaT(PRECISION T){
    return Cr * T * T;
}

// heat capacity density
PRECISION Cp(PRECISION s, PRECISION rhob, PRECISION corrL2){
    return (s * s / rhob) * (corrL2 / xi02);
}

// correlation length
PRECISION xi(PRECISION T, PRECISION muB){
    //PRECISION deltaE = e - ec;
    //PRECISION deltaRhob = rhob - rhobc;
    //return xi0 * exp(- deltaE * deltaE/(2*sigmae*sigmae) - deltaRhob * deltaRhob/(2*sigman*sigman)) + xi0; // to be fixed
    return correlationLength(T, muB);
}

// derivative of log(xi) with respect to energy
PRECISION dlnXide(PRECISION e, PRECISION rhob){
    return dLnxidE(e, rhob);
    //return -((e - ec)/(sigmae*sigmae)); // to be fixed
}

// derivative of log(xi) with respect to baryon density
PRECISION dlnXidrhob(PRECISION e, PRECISION rhob){
    return dLnxidN(e, rhob);
    //return -((rhob - rhobc)/(sigman*sigman)); // to be fixed
}

// derivative of log(phi0) with respect to energy
PRECISION dlnPhi0de(PRECISION T, PRECISION s, PRECISION dlnXi_de){
    return 2/(s*T) + 2 * dlnXi_de;
}

// derivative of log(phi0) with respect to baryon density
PRECISION dlnPhi0drhob(PRECISION alphaB, PRECISION rhob, PRECISION s, PRECISION dlnXi_drhob){
    return -2 * alphaB / s - 3 / rhob + 2 * dlnXi_drhob;
}

/**************************************************************************************************************************************************/
/* Conductivity, heat capacity, correlation length and relaxation coefficients of different slow modes, and their derivatives
/**************************************************************************************************************************************************/

// universal function
PRECISION f2(PRECISION x){
    return 1.0 / (1.0 + x*x); // Eq. (93)
}

// relaxation coefficents of fluctuations, without (Q*xi)f2(Q*xi), just 2*lambdaT/(Cp*xi^2).
PRECISION relaxationCoefficientPhi(PRECISION rhob, PRECISION s, PRECISION T, PRECISION corrL2)
{
    PRECISION lambdat = lambdaT(T);
    PRECISION cp = Cp(s, rhob, corrL2);
    
    return 2 * lambdat/(cp * corrL2);
}

// relaxation coefficents of fluctuations, only work for f2 defined above.
PRECISION relaxationCoefficientPhiQ(PRECISION gammaPhi, PRECISION corrL2, PRECISION Q)
{
    PRECISION Q2 = Q * Q;
    PRECISION qL2 = corrL2 * Q2;
    PRECISION qL4 = qL2 * qL2;
    
    return gammaPhi * (qL2 + qL4);
}


/**************************************************************************************************************************************************/
/* slow modes out and at equilibrium with different Q
/**************************************************************************************************************************************************/

// slow modes with zero Q
PRECISION equilibriumPhi0(PRECISION rhob, PRECISION s, PRECISION corrL2)
{
    return Cp(s, rhob, corrL2) / (rhob * rhob); // slow modes at equilibrium with Q = 0, Eq.(90)
}

// slow modes with nonzero Q
PRECISION equilibriumPhiQ(PRECISION e, PRECISION rhob, PRECISION T, PRECISION muB, PRECISION s, PRECISION Q)
{
    PRECISION corrL = xi(T, muB);
    PRECISION corrL2 = corrL * corrL;
    PRECISION qL = Q * corrL;
    
    return equilibriumPhi0(rhob, s, corrL2) * f2(qL); // Magnitude of mode Q at Equilibrium, Eq. (89)
}

// initialization of slow modes
void setInitialConditionSlowModes(void * latticeParams, void * hydroParams)
{
    struct LatticeParameters * lattice = (struct LatticeParameters *) latticeParams;
    struct HydroParameters * hydro = (struct HydroParameters *) hydroParams;
    int nx = lattice->numLatticePointsX;
    int ny = lattice->numLatticePointsY;
    int nz = lattice->numLatticePointsRapidity;
    
#ifdef HydroPlus
    printf("Hydro+ is on, number of slow modes is %d, Q0 is %f, dQ is %f...\n",NUMBER_SLOW_MODES, Q0, dQvec);
    
    // initialization of Q vectors
    for(unsigned int n = 0; n < NUMBER_SLOW_MODES; ++n){
        Qvec[n] = Q0 + n * dQvec;
    }

    // initialization of slow mdoes at/out of equilibrium
    for(int i = 2; i < nx+2; ++i) {
        for(int j = 2; j < ny+2; ++j) {
            for(int k = 2; k < nz+2; ++k) {
                
                int s = columnMajorLinearIndex(i, j, k, nx+4, ny+4);
                
                PRECISION es = e[s];
                PRECISION rhobs = rhob[s];
                PRECISION Ts = T[s];
                PRECISION muBs = Ts * alphaB[s];
                PRECISION seqs = seq[s];
                
                for(unsigned int n = 0; n < NUMBER_SLOW_MODES; ++n){
                    
                    PRECISION equiPhiQ = equilibriumPhiQ(es, rhobs, Ts, muBs, seqs, Qvec[n]);
                    
                    eqPhiQ->phiQ[n][s] = equiPhiQ;
                    q->phiQ[n][s] = equiPhiQ;
                    
                    //printf("Initialization: equiPhiQ = %f\n",equiPhiQ);
                }
            }
        }
    }
#endif
}

/**************************************************************************************************************************************************/
/* contributions from slow modes to inferred variables for a specific cell
/**************************************************************************************************************************************************/

// note: the integrands of alpha and beta only work for f2 defined above.
// this function takes e/p/rhob/T/alphaB and slow modes PhiQ/eqPhiQ, then returns variables with contributions from slow modes, including p/T/alphaB
void getPressurePlusFromSlowModes(PRECISION * const __restrict__ pPlus, const PRECISION * const __restrict__ equiPhiQ, const PRECISION * const __restrict__ PhiQ, PRECISION e, PRECISION rhob, PRECISION p, PRECISION T, PRECISION alphaB, PRECISION s)
{

    PRECISION muB = alphaB * T;
    PRECISION corrL = xi(T, muB); // correlation length
    PRECISION corrL2 = corrL * corrL;
    
    PRECISION heatC = Cp(s, rhob, corrL2); // heat capacity
    
    // derivaties
    PRECISION dlnXi_de = dlnXide(e, rhob);
    PRECISION dlnXi_drhob = dlnXidrhob(e, rhob);
    PRECISION dlnPhi0_de = dlnPhi0de(T, s, dlnXi_de);
    PRECISION dlnPhi0_drhob = dlnPhi0drhob(alphaB, rhob, s, dlnXi_drhob);
    
    PRECISION entropy = 0.0;
    PRECISION alpha = 0.0;
    PRECISION beta = 0.0;
    
    // dQ/(2*pi)^2
    PRECISION facQ = dQvec/(4 * M_PI * M_PI);
    
    // contributions from slow modes to alpha, beta and entropy
    for(unsigned int n = 0; n < NUMBER_SLOW_MODES; ++n){
        
        // ln(Phi/eqPhi) and (Phi/eqPhi-1)
        PRECISION phiRatio = PhiQ[n] / (equiPhiQ[n] + 1e-15);
        PRECISION phiRatioLog = log(phiRatio);
        PRECISION phiRatioOne = phiRatio - 1;
        //printf("PhiQ[n]=%f\t equiPhiQ[n]=%f\t phiRatio=%f\t phiRatioLog=%f\n",PhiQ[n],equiPhiQ[n],phiRatio,phiRatioLog);

        // (Q*xi)f2(Q*xi), Riemann sum for the intergal below
        PRECISION Q = Qvec[n] + 0.5 * dQvec;
        PRECISION qL = Q * corrL;
        PRECISION qL2= qL * qL;
        PRECISION qLf2 = qL / (1 + qL2);
        
        // Q^2
        PRECISION Q2 = Q * Q;
        // Q^2*(Phi/eqPhi-1)
        PRECISION QphiRatioOne = Q2 * phiRatioOne;
        // Q^2*ln(Phi/eqPhi)
        PRECISION QphiRatioLog = Q2 * log(phiRatio);
        
        // delta beta, Eq.(106)
        PRECISION intBeta = QphiRatioOne * (dlnPhi0_de - 2 * qLf2 * dlnXi_de);
        beta += intBeta;
        
        // delta alpha, Eq.(106)
        PRECISION intAlpha = QphiRatioOne * (dlnPhi0_drhob - 2 * qLf2 * dlnXi_drhob);
        alpha += intAlpha;
        
        // delta entropy, Eq.(85)
        PRECISION intEntropy = QphiRatioLog - QphiRatioOne;
        entropy += intEntropy;
    }
    
    // contributions from slow modes to entropy, inverse temperature, chemical potential over temperature and pressure
    PRECISION deltaS = facQ * entropy;
    PRECISION deltaAlphaB = - facQ * alpha;
    PRECISION deltaBeta = facQ * beta;
    
    //printf("deltaS=%f,\t deltaAlphaB=%f,\t deltaBeta=%f.\n",deltaS,deltaAlphaB,deltaBeta);
    
    // variables(+) with contribution from slow modes
    T = 1 / (1/T + deltaBeta);
    
    PRECISION deltaP = T * (deltaS - (e + p) * deltaBeta + rhob * deltaAlphaB);
    *pPlus = p;// + deltaP;
    //printf("corrL=%f\t dlnXi_de=%f\t muB=%f\t deltaP=%f\t p+=%f\n",corrL,dlnXi_de,muB,deltaP,p+deltaP);
}

/**************************************************************************************************************************************************/
/* dlnxide, dlnxidn, correlation length table
/**************************************************************************************************************************************************/

PRECISION dLnxidE(PRECISION e0, PRECISION rhob0){
    PRECISION ep, em, xip, xim, Tp, Tm, muBp, muBm;
    PRECISION delta_e = 0.02;
    
    ep = e0 + 0.1 * delta_e;
    em = e0 - 0.1 * delta_e;
    
    if(em >= 0 && ep <= 18.0)
    {
        PRECISION mPrimaryVariables[3], pPrimaryVariables[3];
        
        getPrimaryVariablesCombo(em, rhob0, mPrimaryVariables);
        getPrimaryVariablesCombo(ep, rhob0, pPrimaryVariables);
        
        Tm = mPrimaryVariables[1];
        muBm = Tm * mPrimaryVariables[2];
        xim = correlationLength(Tm, muBm);
        
        Tp = pPrimaryVariables[1];
        muBp = Tp * pPrimaryVariables[2];
        xip = correlationLength(Tp, muBp);
        
        return (log(xip)-log(xim))/(2 * 0.1 * delta_e);
    }
    else{
        return 0.0;
    }
}

PRECISION dLnxidN(PRECISION e0, PRECISION rhob0){
    PRECISION np, nm, xip, xim, Tp, Tm, muBp, muBm;
    PRECISION delta_n = 0.005;
    
    np = rhob0 + delta_n;
    nm = rhob0 - delta_n;
    
    if(nm >= 0 && np <= 0.9)
    {
        PRECISION mPrimaryVariables[3], pPrimaryVariables[3];
        
        getPrimaryVariablesCombo(e0, nm, mPrimaryVariables);
        getPrimaryVariablesCombo(e0, np, pPrimaryVariables);
        
        Tm = mPrimaryVariables[1];
        muBm = Tm * mPrimaryVariables[2];
        xim = correlationLength(Tm, muBm);
        
        Tp = pPrimaryVariables[1];
        muBp = Tp * pPrimaryVariables[2];
        xip = correlationLength(Tp, muBp);
        
        return (log(xip)-log(xim))/(2 * delta_n);
    }
    else{
        return 0.0;
    }
}


// Read in the table of correlation length
void getCorrelationLengthTable(){

#ifdef CRITICAL
    // correlation length table
    FILE *filexi;
    PRECISION x, y;
    
    filexi = fopen ("../Tmu_table/xivsmuT.dat","r");
    if(filexi==NULL){
        printf("xivsmuT.dat was not opened...\n");
        exit(-1);
    }
    else
    {
        fseek(filexi,0L,SEEK_SET);
        for(int i = 0; i < 9396; ++i){
            fscanf(filexi,"%lf %lf %lf", & x, & y, & xieq[i]);
        }
    }
    fclose(filexi);
    
    // make derivatives of correlation length tables
    FILE *filedxi;
    filedxi = fopen ("input/dxi.dat","w");

    float de = 0.02;
    float dn = 0.005;
    
    for(int i = 0; i < 301; ++i) {
        for(int j = 0; j < 81; ++j) {
            
            x = i * de;
            y = j * dn;

            float dlogxide = dLnxidE(x,y);
            float dlogxidn = dLnxidN(x,y);
            
            //if(dlogxide<0.0||dlogxidn<0.0) printf("e=%f,\t rhob=%f,\t dlogxide=%f,\t dlogxidn=%f\n",x,y,dlogxide,dlogxidn);
            
            fprintf(filedxi, "%.3f\t%.3f\t%.3f\t%.3f\n",x,y,dlogxide,dlogxidn);
        }
    }
    
    fclose(filedxi);
    
    // read derivative table
    /*filedxi = fopen ("input/dxi.dat","r");
    if(filedxi==NULL){
        printf("dxi.dat was not opened...\n");
        exit(-1);
    }
    else
    {
        fseek(filedxi,0L,SEEK_SET);
        for(int i = 0; i < 163081; ++i){
            fscanf(filedxi,"%lf %lf %lf", & x, & y, & dlnxide[i], & dlnxidn[i]);
        }
    }
    
    fclose(filedxi);

    printf("Correlation length tables are read in.\n");
    
    // correlation length as a function of (e, rhob)*/
//#ifdef CRITICAL_D
    char xitable[] = "output/xitable_enb.dat";
    ofstream xifile(xitable);
    for(int i = 0; i < 900; ++i) {
        for(int j = 0; j < 180; ++j) {
            
            float e = i * 0.02;
            float rhob = j * 0.005;
            
            PRECISION PrimaryVariables[3];
            
            getPrimaryVariablesCombo(e, rhob, PrimaryVariables);
            
            //float peq = //PrimaryVariables[0];
            float Teq = PrimaryVariables[1];//effectiveTemperature(e, rhob);//
            float alphaBeq = PrimaryVariables[2];//chemicalPotentialOverT(e, rhob);//
            float muB = alphaBeq*Teq;
            
            float xi = correlationLength(Teq, muB);
            float logxi = log(xi);
            
            if(logxi<0.0) printf("Teq=%f,\t muB=%f,\t xi=%f,\t lnxi=%f\n",Teq,muB,xi,logxi);
            
            xifile
            << setprecision(5) << setw(10) << e//*HBARC
            << setprecision(5) << setw(10) << rhob
            << setprecision(6) << setw(18) << Teq*HBARC
            << setprecision(6) << setw(18) << muB*HBARC
            << setprecision(6) << setw(18) << logxi
            << endl;
        }
    }
    xifile.close();
//#endif
    
#endif
}

PRECISION correlationLength(PRECISION T, PRECISION muB){
    PRECISION T0 = T*HBARC;
    PRECISION muB0 = muB*HBARC;
    
    if((0.08<=T0)&&(T0<=0.24)){
        if((0.22<=muB0)&&(muB0<=0.45)){
            return InferredPrimaryVariable(muB0, T0-0.08, 0.22, 0.002, 81, 0.002, 0, xieq);
        }
        else
            return 1.0;
    }else
        return 1.0;
}
