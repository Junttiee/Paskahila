#ifndef __AVG_FIELD_STRENGTH__CPP__
#define __AVG_FIELD_STRENGTH__CPP__

///////////////////////////////////////////////////
// COMPUTATION OF ADJOINT FOR PARALLEL TRANSPORT //
///////////////////////////////////////////////////

#define SET_ADJOINT_BUFFERS() \
SU_Nc_ADJOINT_FORMAT U0Adjx[SUNcAlgebra::NumberOfGenerators*SUNcAlgebra::NumberOfGenerators];\
SU_Nc_ADJOINT_FORMAT U1Adjy[SUNcAlgebra::NumberOfGenerators*SUNcAlgebra::NumberOfGenerators];\
SU_Nc_ADJOINT_FORMAT U2Adjz[SUNcAlgebra::NumberOfGenerators*SUNcAlgebra::NumberOfGenerators];\
\
SU_Nc_ADJOINT_FORMAT U0AdjxM[SUNcAlgebra::NumberOfGenerators*SUNcAlgebra::NumberOfGenerators];\
SU_Nc_ADJOINT_FORMAT U1AdjyM[SUNcAlgebra::NumberOfGenerators*SUNcAlgebra::NumberOfGenerators];\
SU_Nc_ADJOINT_FORMAT U2AdjzM[SUNcAlgebra::NumberOfGenerators*SUNcAlgebra::NumberOfGenerators];

#define COMPUTE_NEIGHBORING_ADJOINTS(x,y,z) \
SUNcGroup::Operations::GetAdjoint(U->Get(x,y,z,0),U0Adjx); \
SUNcGroup::Operations::GetAdjoint(U->Get(x,y,z,1),U1Adjy); \
SUNcGroup::Operations::GetAdjoint(U->Get(x,y,z,2),U2Adjz); \
\
SUNcGroup::Operations::GetAdjoint(U->Get(x-1,y,z,0),U0AdjxM); \
SUNcGroup::Operations::GetAdjoint(U->Get(x,y-1,z,1),U1AdjyM); \
SUNcGroup::Operations::GetAdjoint(U->Get(x,y,z-1,2),U2AdjzM);

////////////////////////////////////////////////////////
// BUFFERS FOR COMPUTATION OF AVERAGE FIELD STRENGTHS //
////////////////////////////////////////////////////////

// MAGNETIC //
#define SET_AVG_MAGNETIC_FIELD_STRENGTH_BUFFERS() \
\
SET_ELEMENTARY_PLAQUETTE_BUFFERS(); \
SET_NEIGHBORING_PLAQUETTE_BUFFERS(); \
SET_EXTENDED_PLAQUETTE_BUFFERS(); \
SET_MOVED_PLAQUETTE_BUFFERS(); \
SET_ELEMENTARY_COLOR_TRACE_BUFFERS(); \
SET_MOVED_COLOR_TRACES_BUFFERS(); \
SET_SECOND_PWR_PLAQUETTE_BUFFERS(); \
\
SU_Nc_FUNDAMENTAL_FORMAT UzxAvg[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UxyAvg[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UyzAvg[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UxyAvgmpx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UzxAvgmpx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UyzAvgmpx[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UxyAvgmpy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UzxAvgmpy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UyzAvgmpy[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UxyAvgmpz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UzxAvgmpz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UyzAvgmpz[SUNcGroup::MatrixSize]; \
\
DOUBLE B0Loc[SUNcAlgebra::NumberOfGenerators];\
DOUBLE B1Loc[SUNcAlgebra::NumberOfGenerators];\
DOUBLE B2Loc[SUNcAlgebra::NumberOfGenerators];\
\
DOUBLE B0SqrLoc,B1SqrLoc,B2SqrLoc; \
DOUBLE B0SqrLocmpx,B1SqrLocmpx,B2SqrLocmpx; \
DOUBLE B0SqrLocmpy,B1SqrLocmpy,B2SqrLocmpy; \
DOUBLE B0SqrLocmpz,B1SqrLocmpz,B2SqrLocmpz;

// ELECTRIC //
#define SET_AVG_ELECTRIC_FIELD_STRENGTH_BUFFERS() \
\
DOUBLE E0Loc[SUNcAlgebra::NumberOfGenerators];\
DOUBLE E1Loc[SUNcAlgebra::NumberOfGenerators];\
DOUBLE E2Loc[SUNcAlgebra::NumberOfGenerators];\
\
DOUBLE E0SqrLoc,E1SqrLoc,E2SqrLoc; \

//DOUBLE TrUE0Umpx=0.0;  DOUBLE TrUE1Umpy=0.0;  DOUBLE TrUE2Umpz=0.0;

// ALL //
#define SET_AVG_FIELD_STRENGTH_BUFFERS() \
\
SET_AVG_MAGNETIC_FIELD_STRENGTH_BUFFERS();\
SET_AVG_ELECTRIC_FIELD_STRENGTH_BUFFERS();


/////////////////////////////////////////////////////////////
// COMPUTATION OF AVERAGE FIELD STRENGTHS AT POINT (X,Y,Z) //
/////////////////////////////////////////////////////////////

// MAGNETIC //
#define COMPUTE_AVG_MAGNETIC_FIELD_STRENGTH(x,y,z,U) \
\
COMPUTE_ELEMENTARY_PLAQUETTES(x,y,z,U); \
COMPUTE_NEIGHBORING_PLAQUETTES(x,y,z,U); \
COMPUTE_EXTENDED_PLAQUETTES(x,y,z,U); \
COMPUTE_ELEMENTARY_PLAQUETTES(x,y,z,U); \
COMPUTE_MOVED_PLAQUETTES(x,y,z,U); \
COMPUTE_ELEMENTARY_COLOR_TRACES(); \
COMPUTE_MOVED_COLOR_TRACES(); \
COMP_SECOND_PWR_U_BUFFERS(x,y,z,U); \
\
SUNcGroup::AvgMatrix(Uzx,UMzx,UzMx,UMzMx,UzxAvg); \
SUNcGroup::AvgMatrix(Uxy,UMxy,UxMy,UMxMy,UxyAvg); \
SUNcGroup::AvgMatrix(Uyz,UMyz,UyMz,UMyMz,UyzAvg); \
\
SUNcGroup::AvgMatrix(Uxympx,UMxympx,UxMympx,UMxMympx,UxyAvgmpx); \
SUNcGroup::AvgMatrix(Uzxmpx,UMzxmpx,UzMxmpx,UMzMxmpx,UzxAvgmpx); \
SUNcGroup::AvgMatrix(Uyzmpx,UMyzmpx,UyMzmpx,UMyMzmpx,UyzAvgmpx); \
\
SUNcGroup::AvgMatrix(Uxympy,UMxympy,UxMympy,UMxMympy,UxyAvgmpy); \
SUNcGroup::AvgMatrix(Uzxmpy,UMzxmpy,UzMxmpy,UMzMxmpy,UzxAvgmpy); \
SUNcGroup::AvgMatrix(Uyzmpy,UMyzmpy,UyMzmpy,UMyMzmpy,UyzAvgmpy); \
\
SUNcGroup::AvgMatrix(Uxympz,UMxympz,UxMympz,UMxMympz,UxyAvgmpz); \
SUNcGroup::AvgMatrix(Uzxmpz,UMzxmpz,UzMxmpz,UMzMxmpz,UzxAvgmpz); \
SUNcGroup::AvgMatrix(Uyzmpz,UMyzmpz,UyMzmpz,UMyMzmpz,UyzAvgmpz); \
\
SUNcGroup::Operations::ReTrIGenU(DOUBLE(2.0),UyzAvg,B0Loc); \
SUNcGroup::Operations::ReTrIGenU(DOUBLE(2.0),UzxAvg,B1Loc); \
SUNcGroup::Operations::ReTrIGenU(DOUBLE(2.0),UxyAvg,B2Loc); \
\
B0SqrLoc=DOUBLE(4.0)*SUNcGroup::Operations::ReTrIDMinusU(UyzAvg); \
B1SqrLoc=DOUBLE(4.0)*SUNcGroup::Operations::ReTrIDMinusU(UzxAvg); \
B2SqrLoc=DOUBLE(4.0)*SUNcGroup::Operations::ReTrIDMinusU(UxyAvg); \
\
B2SqrLocmpx=SUNcGroup::Operations::ReTrIDMinusU(UxyAvgmpx); \
B1SqrLocmpx=SUNcGroup::Operations::ReTrIDMinusU(UzxAvgmpx); \
B0SqrLocmpx=SUNcGroup::Operations::ReTrIDMinusU(UyzAvgmpx); \
\
B2SqrLocmpy=SUNcGroup::Operations::ReTrIDMinusU(UxyAvgmpy); \
B1SqrLocmpy=SUNcGroup::Operations::ReTrIDMinusU(UzxAvgmpy); \
B0SqrLocmpy=SUNcGroup::Operations::ReTrIDMinusU(UyzAvgmpy); \
\
B2SqrLocmpz=SUNcGroup::Operations::ReTrIDMinusU(UxyAvgmpz); \
B1SqrLocmpz=SUNcGroup::Operations::ReTrIDMinusU(UzxAvgmpz); \
B0SqrLocmpz=SUNcGroup::Operations::ReTrIDMinusU(UyzAvgmpz); 


// ELECTRIC //
#define COMPUTE_AVG_ELECTRIC_FIELD_STRENGTH(x,y,z)\
\
SU_Nc_ALGEBRA_FORMAT E0Down[SUNcAlgebra::NumberOfGenerators]; \
SU_Nc_ALGEBRA_FORMAT E1Down[SUNcAlgebra::NumberOfGenerators]; \
SU_Nc_ALGEBRA_FORMAT E2Down[SUNcAlgebra::NumberOfGenerators]; \
\
SUNcAlgebra::Operations::InverseAdjointMultiplication(U->Get(x-1,y,z,0),E->Get(x-1,y,z,0,0),E0Down);\
SUNcAlgebra::Operations::InverseAdjointMultiplication(U->Get(x,y-1,z,1),E->Get(x,y-1,z,1,0),E1Down);\
SUNcAlgebra::Operations::InverseAdjointMultiplication(U->Get(x,y,z-1,2),E->Get(x,y,z-1,2,0),E2Down);\
\
for(int a=0;a<SUNcAlgebra::NumberOfGenerators;a++){\
E0Loc[a]=DOUBLE(0.5)*(E->Get(x,y,z,0,a)[0]+E0Down[a]); \
E1Loc[a]=DOUBLE(0.5)*(E->Get(x,y,z,1,a)[0]+E1Down[a]); \
E2Loc[a]=DOUBLE(0.5)*(E->Get(x,y,z,2,a)[0]+E2Down[a]); \
} \
\
E0SqrLoc=0.0;   E1SqrLoc=0.0;   E2SqrLoc=0.0; \
\
for(int a=0;a<SUNcAlgebra::NumberOfGenerators;a++){\
\
E0SqrLoc+=DOUBLE(0.5)*(SQR(E->Get(x,y,z,0,a)[0])+SQR(E->Get(x-1,y,z,0,a)[0])); \
E1SqrLoc+=DOUBLE(0.5)*(SQR(E->Get(x,y,z,1,a)[0])+SQR(E->Get(x,y-1,z,1,a)[0])); \
E2SqrLoc+=DOUBLE(0.5)*(SQR(E->Get(x,y,z,2,a)[0])+SQR(E->Get(x,y,z-1,2,a)[0]));} \
\


//This one could be for the T_kk_mod Electric parts
// U_{k,m}^dag * E_{m}^{k} * U_{k,m}
#define COMPUTE_AVG_ELECTRIC_FIELD_STR_MOD(x,y,z)\
\
SU_Nc_ALGEBRA_FORMAT E0Up[SUNcAlgebra::NumberOfGenerators]; \
SU_Nc_ALGEBRA_FORMAT E1Up[SUNcAlgebra::NumberOfGenerators]; \
SU_Nc_ALGEBRA_FORMAT E2Up[SUNcAlgebra::NumberOfGenerators]; \
\
SUNcAlgebra::Operations::InverseAdjointMultiplication(U->Get(x,y,z,0),E->Get(x,y,z,0,0),E0Up);\
SUNcAlgebra::Operations::InverseAdjointMultiplication(U->Get(x,y,z,1),E->Get(x,y,z,1,0),E1Up);\
SUNcAlgebra::Operations::InverseAdjointMultiplication(U->Get(x,y,z,2),E->Get(x,y,z,2,0),E2Up);\


/* Maybe defining our own function to separate from og
This one could be for T_jk
Here for U_{k,m}*E_{m+k}^{j}*U_{k,m}^dag & U_{j,m}*E_{m+j}^{k}*U_{j,m}^dag:
SUNcAlgebra::Operations::AdjointMultiplication(U->Get(x,y,z,0),E->Get(x+1,y,z,[1,2],0),NAMETHIS);\
SUNcAlgebra::Operations::AdjointMultiplication(U->Get(x,y,z,1),E->Get(x,y+1,z,[0,2],0),NAMETHIS);\
SUNcAlgebra::Operations::AdjointMultiplication(U->Get(x,y,z,2),E->Get(x,y,z+1,[1,2],0),NAMETHIS);\

*/

/*// ELECTRIC //
 #define COMPUTE_AVG_ELECTRIC_FIELD_STRENGTH(x,y,z) \
 \
 for(int a=0;a<SUNcAlgebra::NumberOfGenerators;a++){\
 E0Loc[a]=0.0;   E1Loc[a]=0.0;   E2Loc[a]=0.0;\
 }\
 \
 COMPUTE_NEIGHBORING_ADJOINTS(x,y,z); \
 \
 for(int a=0;a<SUNcAlgebra::NumberOfGenerators;a++){\
 for(int b=0;b<SUNcAlgebra::NumberOfGenerators;b++){ \
 E0Loc[a]+=DOUBLE(0.5)*(DELTA(a,b)*E->Get(x,y,z,0,b)[0]+U0AdjxM[SUNcGroup::AdjIndex(b,a)]*E->Get(x-1,y,z,0,b)[0]); \
 E1Loc[a]+=DOUBLE(0.5)*(DELTA(a,b)*E->Get(x,y,z,1,b)[0]+U1AdjyM[SUNcGroup::AdjIndex(b,a)]*E->Get(x,y-1,z,1,b)[0]); \
 E2Loc[a]+=DOUBLE(0.5)*(DELTA(a,b)*E->Get(x,y,z,2,b)[0]+U2AdjzM[SUNcGroup::AdjIndex(b,a)]*E->Get(x,y,z-1,2,b)[0]); \
 } \
 }\
 \
 E0SqrLoc=0.0;   E1SqrLoc=0.0;   E2SqrLoc=0.0; \
 \
 for(int a=0;a<SUNcAlgebra::NumberOfGenerators;a++){\
 \
 E0SqrLoc+=DOUBLE(0.5)*(SQR(E->Get(x,y,z,0,a)[0])+SQR(E->Get(x-1,y,z,0,a)[0])); \
 E1SqrLoc+=DOUBLE(0.5)*(SQR(E->Get(x,y,z,1,a)[0])+SQR(E->Get(x,y-1,z,1,a)[0])); \
 E2SqrLoc+=DOUBLE(0.5)*(SQR(E->Get(x,y,z,2,a)[0])+SQR(E->Get(x,y,z-1,2,a)[0])); \
 \
 }*/

// ALL //
#define COMPUTE_AVG_FIELD_STRENGTH(x,y,z,U) \
\
COMPUTE_AVG_MAGNETIC_FIELD_STRENGTH(x,y,z,U);\
COMPUTE_AVG_ELECTRIC_FIELD_STRENGTH(x,y,z);\
COMPUTE_AVG_ELECTRIC_FIELD_STR_MOD(x,y,z);
#endif

