//
//  HydroAnalysis.cpp
//  
//
//  Created by Lipei Du on 10/25/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>//Lipei
#include <istream>//Lipei
#include <fstream>//Lipei
#include <stdio.h>//Lipei
#include <stdlib.h>//Lipei
#include <cassert>//Lipei
#include <string>//Lipei
#include <iomanip>//by Lipei
using namespace std;//Lipei

#include "../include/DynamicalVariables.h"
#include "../include/LatticeParameters.h"
#include "../include/PrimaryVariables.h"
#include "../include/EquationOfState.h"
#include "../include/HydroAnalysis.h"
#include "../include/HydroPlus.h"

#define HBARC 0.197326938
void outputAnalysis(double t, FILE *outputDir, void * latticeParams)
{
    /*FILE *fp;
    char fname[255];
    sprintf(fname, "%s/AnalysisData.dat", outputDir);
    fp=fopen(fname, "a+");*/
    
    struct LatticeParameters * lattice = (struct LatticeParameters *) latticeParams;
    int nx = lattice->numLatticePointsX;
    int ny = lattice->numLatticePointsY;
    int nz = lattice->numLatticePointsRapidity;
    double dx = lattice->latticeSpacingX;
    double dy = lattice->latticeSpacingY;
    double dz = lattice->latticeSpacingRapidity;
    
    double x,y,z;
    
    int i,j,k;
    int s;
    
    /*double v2t,becc,eecc,v2t1,v2t2;
    v2t = 0;
    v2t1 = 0;
    v2t2 = 0;
    becc = 0;
    eecc = 0;
    double bymx = 0;
    double bxy = 0;
    double bypx = 0;
    double eymx = 0;
    double exy = 0;
    double eypx = 0;*/
    
    //PRECISION phiQ;
    
    //k=(nz+3)/2;
    //j=(ny+3)/2;
    //i=(nx+3)/2;
    for(i = 2; i < nx+2; ++i) {
        for(j = 2; j < ny+2; ++j) {
            
            PRECISION mub0,t0,mub1,t1,mub2,t2,mub3,t3,mub4,t4;
            x = (i-2 - (nx-1)/2.) * dx;
            y = (j-2 - (ny-1)/2.) * dy;
            
            if(x==0&&y==0){
                
            for(k = 2; k < nz+2; ++k) {
                
                z = (k-2 - (nz-1)/2.) * dz;
                
                s = columnMajorLinearIndex(i, j, k, nx+4, ny+4);
                
                if(z==0.0){
                    t0 = T[s];
                    mub0 = t0*alphaB[s];
                }
                if(z==0.5){
                    t1 = T[s];
                    mub1 = t1*alphaB[s];
                }
                if(z==1.0){
                    t2 = T[s];
                    mub2 = t2*alphaB[s];
                }
                if(z==1.5){
                    //printf("here!\n");
                    t3 = T[s];
                    mub3 = t3*alphaB[s];
                }
                if(fabs(z-1.7)<1.e-3){
                    //printf("here\n");
                    t4 = T[s];
                    mub4 = t4*alphaB[s];
                }
                
                
                
                
                    
                
                //if(x==0.0&&y==0.0&&t==1.5)
                //    fprintf(fp, "%.3f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\n",z,e[s],rhob[s],seq[s],alphaB[s],T[s]);
                
                //if(x==0&&y==0)
                //fprintf(fp, "%.3f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\n",t,e[s],q->pinn[s],q->Pi[s],rhob[s],q->nbn[s],T[s],p[s]);
                
                //if(x==0&&y==0)
                    //fprintf(fp, "%.3f\t%.8f\t%.8f\t%.8f\n",t,e[s],q->pinn[s],p[s]);
                
                //double tt=Ttt(e[s],p[s],u->ut[s],q->pitt[s]);
                //double tx=Ttx(e[s],p[s],u->ut[s],u->ux[s],q->pitx[s]);
                //double ty=Tty(e[s],p[s],u->ut[s],u->uy[s],q->pity[s]);
                //double tn=Ttn(e[s],p[s],u->ut[s],u->un[s],q->pitn[s]);
/*#ifndef PIMUNU
                double pixx=0;
                double piyy=0;
#else
                double pixx=q->pixx[s];
                double piyy=q->piyy[s];
#endif
                double xx=Txx(e[s],p[s],u->ux[s],pixx);
                //double xy=Txy(e[s],p[s],u->ux[s],u->uy[s],q->pixy[s]);
                //double xn=Txn(e[s],p[s],u->ux[s],u->un[s],q->pixn[s]);
                double yy=Tyy(e[s],p[s],u->uy[s],piyy);
                //double yn=Tyn(e[s],p[s],u->uy[s],u->un[s],q->piyn[s]);
                //double nn=Tnn(e[s],p[s],u->un[s],q->pinn[s],t);
                
                bymx = bymx + (y*y - x*x)*rhob[s];
                bxy = bxy + x*y*rhob[s];
                bypx = bypx + (y*y + x*x)*rhob[s];
                eymx = eymx + (y*y - x*x)*e[s];
                exy = exy + x*y*e[s];
                eypx = eypx + (y*y + x*x)*e[s];
                v2t1 = v2t1 + (xx-yy);
                v2t2 = v2t2 + (xx+yy);*/
                
                //if(x==0&&y==0)
               // phiQ = q->phiQ[0][s];
                
                /*PRECISION eIn = e[s];
                PRECISION rhobIn = rhob[s];
                PRECISION pIn = p[s];
                PRECISION TIn = T[s];
                PRECISION alphaBIn = alphaB[s];
                PRECISION equiPhiQ[NUMBER_SLOW_MODES];
                PRECISION PhiQ[NUMBER_SLOW_MODES];
                
                for(unsigned int n = 0; n < NUMBER_SLOW_MODES; ++n){
                    
                    equiPhiQ[n] = eqPhiQ->phiQ[n][s];
                    PhiQ[n] = q->phiQ[n][s];//for test
                }
                
                PRECISION p,T,alphaB;

                getPrimaryVariablesFromSlowModes(&p, &T, &alphaB, equiPhiQ, PhiQ, eIn, rhobIn, pIn, TIn, alphaBIn);*/
            }
                fprintf(outputDir, "%.3f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\n",t,mub0,t0,mub1,t1,mub2,t2,mub3,t3,mub4,t4);
            }
            
            
        }
    }
    
    /*becc = sqrt(bymx*bymx + 4*bxy*bxy)/bypx;
    eecc = sqrt(eymx*eymx + 4*exy*exy)/eypx;
    v2t = v2t1/v2t2;
    
    fprintf(fp, "%.3f\t%.8f\t%.8f\t%.8f\n",t,v2t,becc,eecc);*/
    
    //fprintf(fp, "%.3f\t%.8f\n",t,phiQ);
    //if(t>20)
    //exit(0);
    
    //fclose(fp);
}

// To test the interpolatin function to see it reproduce the EOS table
void testEOS(){
    
    char EOStable[] = "output/sigmaB_test.dat";
    ofstream eos_table(EOStable);
    //char EOStable1[] = "output/EOS_t_test5.dat";
    //ofstream eos_table1(EOStable1);
    //char EOStable2[] = "output/eos_dpdrhob_test5.dat";
    //ofstream eos_table2(EOStable2);
    //char EOStable3[] = "output/eos_p_test5.dat";
    //ofstream eos_table3(EOStable3);
    //char EOStable4[] = "output/eos_cs2_test5.dat";
    //ofstream eos_table4(EOStable4);
    
    for(int i = 0; i < 100; ++i) {
        for(int j = 0; j < 130; ++j){
            PRECISION ttest=(0.5+j*5)/HBARC/1000.0;
            PRECISION mubtest=i*5/HBARC/1000.0;
            //printf("ttest=%lf, mubtest=%lf.\n",ttest,mubtest);
            //eos_table4  << setprecision(6) << setw(18) << etest*HBARC << setprecision(6) << setw(18) << rhobtest
            //           << setprecision(6) << setw(18) << speedOfSoundSquared(etest, rhobtest) << endl;
            //eos_table3 << setprecision(6) << setw(18) << etest*HBARC << setprecision(6) << setw(18) << rhobtest
            //           << setprecision(6) << setw(18) << equilibriumPressure(etest, rhobtest)*HBARC << endl;
            //eos_table2 << setprecision(6) << setw(18) << etest*HBARC << setprecision(6) << setw(18) << rhobtest
            //           << setprecision(6) << setw(18) << dPdRhob(etest,rhobtest)*HBARC << endl;
            //eos_table1 << setprecision(6) << setw(18) << etest*HBARC << setprecision(6) << setw(18) << rhobtest
            //           << setprecision(6) << setw(18) << effectiveTemperature(etest, rhobtest)*HBARC << endl;
            eos_table  << setprecision(6) << setw(18) << ttest << setprecision(6) << setw(18) << mubtest
            << setprecision(6) << setw(18) << baryonDiffusionConstant(ttest, mubtest) << endl;
        }
    }
    
    eos_table.close();
    //eos_table1.close();
    //eos_table2.close();
    //eos_table3.close();
    //eos_table4.close();
    printf("EOS table is reproduced.\n");
}

/*PRECISION baryonDiffusionConstant(PRECISION T, PRECISION mub){
 PRECISION T0 = T*HBARC*1000;
 PRECISION mub0 = mub*HBARC*1000;
 if((100<=T0)&&(T0<=450)){
 if((0<=mub0)&&(mub0<=400))
 return InferredPrimaryVariable(mub0, T0-100, 0, 5, 71, 5, 0, 0, EOState->sigmaB)/HBARC/1000 * T;
 else
 return InferredPrimaryVariable(400, T0-100, 0, 5, 71, 5, 0, 0, EOState->sigmaB)/HBARC/1000 * T;
 }else if(T0<100)
 {
 if((0<=mub0)&&(mub0<=400))
 return InferredPrimaryVariable(mub0, 0, 0, 5, 71, 5, 0, 0, EOState->sigmaB)/HBARC/1000 * T;
 else
 return 0.0543361/HBARC/1000 * T;
 }else
 {
 if((0<=mub0)&&(mub0<=400))
 return InferredPrimaryVariable(mub0, 350, 0, 5, 71, 5, 0, 0, EOState->sigmaB)/HBARC/1000 * 2.28; //2.28 [1/fm^4] = 450 MeV
 else
 return 22.5093/HBARC/1000 * 2.28;
 }
 }*/

