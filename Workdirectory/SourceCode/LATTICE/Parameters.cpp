#ifndef __PARAMETERS__CPP__
#define __PARAMETERS__CPP__

namespace Parameters{
	
	INT LeftNucleus=1;		INT RightNucleus=1;

#if defined(COLLISION_STUDY_BUILD)
#ifndef COLLISION_STUDY_END_T
#error "Set COLLISION_STUDY_END_T when building a collision-study executable."
#endif
	// Fixed simulation-unit source strength for the planned collision study.
	DOUBLE Qs0A=0.125;
	DOUBLE Qs0A_L=0.125;
	DOUBLE Qs0A_R=0.125;
#else
	DOUBLE Qs0A=0.25/sqrt(Lattice::a[0]*Lattice::a[1]);

	//********** with different value of Qs0A for right and left nuclei ****//
	DOUBLE Qs0A_L=0.25/sqrt(Lattice::a[0]*Lattice::a[1]);

	DOUBLE Qs0A_R=0.25/sqrt(Lattice::a[0]*Lattice::a[1]);
#endif

	//********** with different value of Qs0A for right and left nuclei ****//

	DOUBLE Qs0R=0.5;

    // UV REGULARISATIONS //
#if defined(COLLISION_STUDY_BUILD)
    DOUBLE Lambda=0.625;
#else
    DOUBLE Lambda=5*Qs0A;
#endif
    
    // IR CUT-OFF //
#if defined(COLLISION_STUDY_BUILD)
	DOUBLE MEff=0.125;
#else
	DOUBLE MEff=Qs0A;
#endif
    
    DOUBLE SigmaLOverAzL=4;
	
	DOUBLE SigmaLOverAzR=4;
	
	//EXPLICITLY FOR PHYSICAL CHARGES//
	//Transverse scales are Qs,a_\perp and R_p. With R_p=2GeV^-1, two dimensionless combination of scales are QsR_p and Qsa_\perp//
	
	DOUBLE SigmaLOverAz=512;
	
	DOUBLE Qs0=0.5; //in Gev calucated from QsRp=1.0 //
			
	INT AtomicNumber=197;
	
	DOUBLE CoMEnergy=500/0.197;												 //in fm^-1
		
	DOUBLE aXInFm=Qs0A*0.197/Qs0;
	
	DOUBLE aYInFm=Qs0A*0.197/Qs0;
	
	DOUBLE aZInFm=aXInFm*Qs0R/(Qs0A*SigmaLOverAz);
	
	DOUBLE aCubeInFm=aXInFm*aYInFm*aZInFm;

	INT Load=0;
	// ENDING TIME FOR THE SMALL LOCAL TEST (ABOUT 100 STEPS AT dT=0.008) //
#if defined(COLLISION_STUDY_BUILD)
	DOUBLE EndSimulationT=COLLISION_STUDY_END_T;
#else
	DOUBLE EndSimulationT=0.8;
#endif
	
}

#endif

//** old simulations done for QsA=1/8  R/a_||=16  N_\perp =128 //

