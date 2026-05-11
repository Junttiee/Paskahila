#ifndef __CALC__PLAQUETTES__CPP__
#define __CALC__PLAQUETTES__CPP__


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                    ELEMENTARY PLAQUETTES                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                 ------<------------<--------   ^                                        //
//                                                 |            ||            |  y|                                        //
//                                                 |            ||            |   |                                        //
//                                                 v U_{-xy}(x) ^v  U_{xy}(x) ^   |                                        //
//                                                 |            ||            |   ------->                                 //
//                                                 |            ||            |         x                                  //
//                                                 |            ||            |                                            //
//                                                 ------>------OO----->-------                                            //
//                                                 ------<------OO----<--------                                            //
//                                                 |            ||            |                                            //
//                                                 |            ||            |                                            //
//                                                 |U_{-x-y}(x) ^| U_{x-y}(x) ^                                            //
//                                                 v            |v            |                                            //
//                                                 |            ||            |                                            //
//                                                 |            ||            |                                            //
//                                                 ------>-------------->------                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////                                            CONVENTIONS                                                       ///////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                         //
//          U_{\mu\nu}(x)=U_{\mu}(x)U_{\nu}(x+\muhat)U_{\mu}^{\dagger}(x+\nuhat)U_{\nu}^{\dagger}(x)                       //
//         U_{-\mu\nu}(x)=U_{\nu}(x)U_{\mu}^{\dagger}(x+\nuhat-\muhat)U_{\nu}^{\dagger}(x-\muhat)U_{\mu}(x-\muhat)         //
//         U_{\mu-\nu}(x)=U_{\nu}^{\dagger}(x-\nuhat)U_{\mu}(x-\nuhat)U_{\nu}(x-\nuhat+\muhat)U_{\mu}^{\dagger}(x)         //
//  U_{-\mu-\nu}(x)=U_{\mu}^{\dagger}(x-\muhat)U_{\nu}^{\dagger}(x-\muhat-\nuhat)U_{\mu}(x-\muhat-\nuhat)U_{\nu}(x-\nuhat) //
//                                                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//BUFFERS FOR THE COMPUTATION OF ELEMENTARY PLAQUETTES
#define SET_ELEMENTARY_PLAQUETTE_BUFFERS() \
\
SU_Nc_FUNDAMENTAL_FORMAT Uzx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uxy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uyz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Uxympx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uxympy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uxympz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Uzxmpx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uzxmpy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uzxmpz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Uyzmpx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uyzmpy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uyzmpz[SUNcGroup::MatrixSize]; 

#define SET_NEIGHBORING_PLAQUETTE_BUFFERS() \
\
SU_Nc_FUNDAMENTAL_FORMAT UMzx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMxy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMyz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UMxympx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMxympy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMxympz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UMzxmpx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMzxmpy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMzxmpz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UMyzmpx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMyzmpy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMyzmpz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UzMx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UyMz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UxMy[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UxMympx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UxMympy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UxMympz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UzMxmpx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UzMxmpy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UzMxmpz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UyMzmpx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UyMzmpy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UyMzmpz[SUNcGroup::MatrixSize]; 

#define SET_EXTENDED_PLAQUETTE_BUFFERS() \
\
SU_Nc_FUNDAMENTAL_FORMAT UMyMz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMxMy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMzMx[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UMxMympx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMxMympy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMxMympz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UMzMxmpx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMzMxmpy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMzMxmpz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT UMyMzmpx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMyMzmpy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UMyMzmpz[SUNcGroup::MatrixSize]; 

// // added by Yannick 
/*Below we use the existing operations and plaquettes to create necessary moved parallel transported plaquettes for T_0k and the ReTr[it^aU] and calculate them.
Also for T_kk terms the necessary plaquttes are created and we calculate the needed ReTr[IDMinusU]. The segmentation faults are dealt with in T_kk with the brilliant method of just not going over the lattice. 
The same could have been used already for T_0k me thinks, but I guess as a testament to my indomitable spirit I kept the way I did it, I guess it shows my understanding of the lattice machinery.  */

// Here for T_0k
#define SET_MOVED_PLAQUETTE_BUFFERS() \
SU_Nc_FUNDAMENTAL_FORMAT Dx_Uxy_Ux[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uymy_UxMy_Dymy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT DxmyUymy_UxMy_DymyUxmy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Dx_Uzx_Ux[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uzmz_UMzx_Dzmz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT DxmzUzmz_UMzx_DzmzUxmz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Dy_Uxy_Uy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uxmx_UMxy_Dxmx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT DymxUxmx_UMxy_DxmxUymx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Dy_Uyz_Uy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uzmz_UyMz_Dzmz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT DymzUzmz_UyMz_DzmzUymz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Dz_Uzx_Uz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uxmx_UzMx_Dxmx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT DzmxUxmx_UzMx_DxmxUzmx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Dz_Uyz_Uz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uymy_UMyz_Dymy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT DzmyUymy_UMyz_DymyUzmy[SUNcGroup::MatrixSize]; 

//BUFFERS FOR THE COMPUTATION OF THE REAL PART OF THE TRACE iT^{a} TIMES THE ELEMENTARY PLAQUETTES
#define SET_ELEMENTARY_COLOR_TRACE_BUFFERS() \
\
DOUBLE ReTrITaUxy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUzx[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUyz[SUNcAlgebra::NumberOfGenerators];

#define SET_NEIGHBORING_COLOR_TRACE_BUFFERS() \
\
DOUBLE ReTrITaUMxy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUMyz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUMzx[SUNcAlgebra::NumberOfGenerators]; \
\
DOUBLE ReTrITaUxMy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUyMz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUzMx[SUNcAlgebra::NumberOfGenerators];


#define SET_EXTENDED_COLOR_TRACE_BUFFERS() \
\
DOUBLE ReTrITaUMxMy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUMyMz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUMzMx[SUNcAlgebra::NumberOfGenerators];



#define SET_MOVED_COLOR_TRACES_BUFFERS()\
\
DOUBLE ReTrITaDx_Uxy_Ux[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUymy_UxMy_Dymy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDxmyUymy_UxMy_DymyUxmy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDx_Uzx_Ux[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUzmz_UMzx_Dzmz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDxmzUzmz_UMzx_DzmzUxmz[SUNcAlgebra::NumberOfGenerators]; \
\
DOUBLE ReTrITaDy_Uxy_Uy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUxmx_UMxy_Dxmx[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDymxUxmx_UMxy_DxmxUymx[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDy_Uyz_Uy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUzmz_UyMz_Dzmz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDymzUzmz_UyMz_DzmzUymz[SUNcAlgebra::NumberOfGenerators]; \
\
DOUBLE ReTrITaDz_Uzx_Uz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUxmx_UzMx_Dxmx[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDzmxUxmx_UzMx_DxmxUzmx[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDz_Uyz_Uz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUymy_UMyz_Dymy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDzmyUymy_UMyz_DymyUzmy[SUNcAlgebra::NumberOfGenerators]; 

//COMPUTATION OF ELEMENTARY PLAQUETTES
#define COMPUTE_ELEMENTARY_PLAQUETTES(x,y,z,U) \
\
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y,z,0),U->Get(x+1,y,z,1),U->Get(x,y+1,z,0),U->Get(x,y,z,1),Uxy); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y,z,1),U->Get(x,y+1,z,2),U->Get(x,y,z+1,1),U->Get(x,y,z,2),Uyz); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y,z,2),U->Get(x,y,z+1,0),U->Get(x+1,y,z,2),U->Get(x,y,z,0),Uzx); \
\
SUNcGroup::AdvancedOperations::UUDD(U->Get(x+1,y,z,0),U->Get(x+2,y,z,1),U->Get(x+1,y+1,z,0),U->Get(x+1,y,z,1),Uxympx); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x+1,y,z,1),U->Get(x+1,y+1,z,2),U->Get(x+1,y,z+1,1),U->Get(x+1,y,z,2),Uyzmpx); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x+1,y,z,2),U->Get(x+1,y,z+1,0),U->Get(x+2,y,z,2),U->Get(x+1,y,z,0),Uzxmpx); \
\
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y+1,z,0),U->Get(x+1,y+1,z,1),U->Get(x,y+2,z,0),U->Get(x,y+1,z,1),Uxympy); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y+1,z,1),U->Get(x,y+2,z,2),U->Get(x,y+1,z+1,1),U->Get(x,y+1,z,2),Uyzmpy); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y+1,z,2),U->Get(x,y+1,z+1,0),U->Get(x+1,y+1,z,2),U->Get(x,y+1,z,0),Uzxmpy); \
\
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y,z+1,0),U->Get(x+1,y,z+1,1),U->Get(x,y+1,z+1,0),U->Get(x,y,z+1,1),Uxympz); \
\
if (z < Lattice::N[2] - 2) { \
    SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y,z+1,2), U->Get(x,y,z+2,0), U->Get(x+1,y,z+1,2), U->Get(x,y,z+1,0), Uzxmpz); \
    SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y,z+1,1), U->Get(x,y+1,z+1,2), U->Get(x,y,z+2,1), U->Get(x,y,z+1,2), Uyzmpz); \
} else { \
    SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y,z,2), U->Get(x,y,z+1,0), U->Get(x+1,y,z,2), U->Get(x,y,z,0), Uzxmpz); \
    SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y,z,1), U->Get(x,y+1,z,2), U->Get(x,y,z+1,1), U->Get(x,y,z,2), Uyzmpz); \
}
// Above If-Else statement used to avoid Segmentation fault with GhostCells at endpoint of lattice in z-dir.

#define COMPUTE_NEIGHBORING_PLAQUETTES(x,y,z,U) \
\
SUNcGroup::AdvancedOperations::UDDU(U->Get(x,y,z,1),U->Get(x-1,y+1,z,0),U->Get(x-1,y,z,1),U->Get(x-1,y,z,0),UMxy); \
SUNcGroup::AdvancedOperations::UDDU(U->Get(x,y,z,2),U->Get(x,y-1,z+1,1),U->Get(x,y-1,z,2),U->Get(x,y-1,z,1),UMyz); \
SUNcGroup::AdvancedOperations::UDDU(U->Get(x,y,z,0),U->Get(x+1,y,z-1,2),U->Get(x,y,z-1,0),U->Get(x,y,z-1,2),UMzx); \
\
SUNcGroup::AdvancedOperations::DUUD(U->Get(x,y-1,z,1),U->Get(x,y-1,z,0),U->Get(x+1,y-1,z,1),U->Get(x,y,z,0),UxMy); \
SUNcGroup::AdvancedOperations::DUUD(U->Get(x,y,z-1,2),U->Get(x,y,z-1,1),U->Get(x,y+1,z-1,2),U->Get(x,y,z,1),UyMz); \
SUNcGroup::AdvancedOperations::DUUD(U->Get(x-1,y,z,0),U->Get(x-1,y,z,2),U->Get(x-1,y,z+1,0),U->Get(x,y,z,2),UzMx); \
\
SUNcGroup::AdvancedOperations::UDDU(U->Get(x+1,y,z,1),U->Get(x,y+1,z,0),U->Get(x,y,z,1),U->Get(x,y,z,0),UMxympx); \
SUNcGroup::AdvancedOperations::UDDU(U->Get(x,y+1,z,1),U->Get(x-1,y+2,z,0),U->Get(x-1,y+1,z,1),U->Get(x-1,y+1,z,0),UMxympy); \
\
SUNcGroup::AdvancedOperations::DUUD(U->Get(x+1,y-1,z,1),U->Get(x+1,y-1,z,0),U->Get(x+2,y-1,z,1),U->Get(x+1,y,z,0),UxMympx); \
SUNcGroup::AdvancedOperations::DUUD(U->Get(x,y,z,1),U->Get(x,y,z,0),U->Get(x+1,y,z,1),U->Get(x,y+1,z,0),UxMympy); \
\
SUNcGroup::AdvancedOperations::UDDU(U->Get(x+1,y,z,2),U->Get(x+1,y-1,z+1,1),U->Get(x+1,y-1,z,2),U->Get(x+1,y-1,z,1),UMyzmpx); \
SUNcGroup::AdvancedOperations::UDDU(U->Get(x,y+1,z,2),U->Get(x,y,z+1,1),U->Get(x,y,z,2),U->Get(x,y,z,1),UMyzmpy); \
\
SUNcGroup::AdvancedOperations::DUUD(U->Get(x+1,y,z-1,2),U->Get(x+1,y,z-1,1),U->Get(x+1,y+1,z-1,2),U->Get(x+1,y,z,1),UyMzmpx); \
SUNcGroup::AdvancedOperations::DUUD(U->Get(x,y+1,z-1,2),U->Get(x,y+1,z-1,1),U->Get(x,y+2,z-1,2),U->Get(x,y+1,z,1),UyMzmpy); \
\
SUNcGroup::AdvancedOperations::UDDU(U->Get(x+1,y,z,0),U->Get(x+2,y,z-1,2),U->Get(x+1,y,z-1,0),U->Get(x+1,y,z-1,2),UMzxmpx);  \
SUNcGroup::AdvancedOperations::UDDU(U->Get(x,y+1,z,0),U->Get(x+1,y+1,z-1,2),U->Get(x,y+1,z-1,0),U->Get(x,y+1,z-1,2),UMzxmpy); \
\
SUNcGroup::AdvancedOperations::DUUD(U->Get(x,y,z,0),U->Get(x,y,z,2),U->Get(x,y,z+1,0),U->Get(x+1,y,z,2),UzMxmpx);\
SUNcGroup::AdvancedOperations::DUUD(U->Get(x-1,y+1,z,0),U->Get(x-1,y+1,z,2),U->Get(x-1,y+1,z+1,0),U->Get(x,y+1,z,2),UzMxmpy); \
\
SUNcGroup::AdvancedOperations::UDDU(U->Get(x,y,z+1,1),U->Get(x-1,y+1,z+1,0),U->Get(x-1,y,z+1,1),U->Get(x-1,y,z+1,0),UMxympz); \
SUNcGroup::AdvancedOperations::DUUD(U->Get(x,y-1,z+1,1),U->Get(x,y-1,z+1,0),U->Get(x+1,y-1,z+1,1),U->Get(x,y,z+1,0),UxMympz); \
SUNcGroup::AdvancedOperations::DUUD(U->Get(x,y,z,2),U->Get(x,y,z,1),U->Get(x,y+1,z,2),U->Get(x,y,z+1,1),UyMzmpz); \
SUNcGroup::AdvancedOperations::UDDU(U->Get(x,y,z+1,0),U->Get(x+1,y,z,2),U->Get(x,y,z,0),U->Get(x,y,z,2),UMzxmpz); \
\
if(z < Lattice::N[2] - 2) { \
    SUNcGroup::AdvancedOperations::UDDU(U->Get(x,y,z+1,2),U->Get(x,y-1,z+2,1),U->Get(x,y-1,z+1,2),U->Get(x,y-1,z+1,1),UMyzmpz); \
    SUNcGroup::AdvancedOperations::DUUD(U->Get(x-1,y,z+1,0),U->Get(x-1,y,z+1,2),U->Get(x-1,y,z+2,0),U->Get(x,y,z+1,2),UzMxmpz); \
} else { \
    SUNcGroup::AdvancedOperations::UDDU(U->Get(x,y,z,2),U->Get(x,y-1,z+1,1),U->Get(x,y-1,z,2),U->Get(x,y-1,z,1),UMyzmpz); \
    SUNcGroup::AdvancedOperations::DUUD(U->Get(x-1,y,z,0),U->Get(x-1,y,z,2),U->Get(x-1,y,z+1,0),U->Get(x,y,z,2),UzMxmpz); \
}
//Above If-Else statement used to avoid Segmentation fault with GhostCells at endpoint of lattice in z-dir. BECAUSE YOU CAN'T CHANGE \
THE GHOSTCELLS ECT OTHER STUFF WITHOUT KNOWING THE WHOLE CODE IN AND OUT

#define COMPUTE_EXTENDED_PLAQUETTES(x,y,z,U) \
\
SUNcGroup::AdvancedOperations::DDUU(U->Get(x-1,y,z,0),U->Get(x-1,y-1,z,1),U->Get(x-1,y-1,z,0),U->Get(x,y-1,z,1),UMxMy); \
SUNcGroup::AdvancedOperations::DDUU(U->Get(x,y-1,z,1),U->Get(x,y-1,z-1,2),U->Get(x,y-1,z-1,1),U->Get(x,y,z-1,2),UMyMz); \
SUNcGroup::AdvancedOperations::DDUU(U->Get(x,y,z-1,2),U->Get(x-1,y,z-1,0),U->Get(x-1,y,z-1,2),U->Get(x-1,y,z,0),UMzMx); \
\
SUNcGroup::AdvancedOperations::DDUU(U->Get(x,y,z,0),U->Get(x,y-1,z,1),U->Get(x,y-1,z,0),U->Get(x+1,y-1,z,1),UMxMympx); \
SUNcGroup::AdvancedOperations::DDUU(U->Get(x-1,y+1,z,0),U->Get(x-1,y,z,1),U->Get(x-1,y,z,0),U->Get(x,y,z,1),UMxMympy); \
\
SUNcGroup::AdvancedOperations::DDUU(U->Get(x+1,y-1,z,1),U->Get(x+1,y-1,z-1,2),U->Get(x+1,y-1,z-1,1),U->Get(x+1,y,z-1,2),UMyMzmpx); \
SUNcGroup::AdvancedOperations::DDUU(U->Get(x,y,z,1),U->Get(x,y,z-1,2),U->Get(x,y,z-1,1),U->Get(x,y+1,z-1,2),UMyMzmpy); \
\
SUNcGroup::AdvancedOperations::DDUU(U->Get(x+1,y,z-1,2),U->Get(x,y,z-1,0),U->Get(x,y,z-1,2),U->Get(x,y,z,0),UMzMxmpx); \
SUNcGroup::AdvancedOperations::DDUU(U->Get(x,y+1,z-1,2),U->Get(x-1,y+1,z-1,0),U->Get(x-1,y+1,z-1,2),U->Get(x-1,y+1,z,0),UMzMxmpy); \
\
SUNcGroup::AdvancedOperations::DDUU(U->Get(x-1,y,z+1,0),U->Get(x-1,y-1,z+1,1),U->Get(x-1,y-1,z+1,0),U->Get(x,y-1,z+1,1),UMxMympz); \
SUNcGroup::AdvancedOperations::DDUU(U->Get(x,y-1,z+1,1),U->Get(x,y-1,z,2),U->Get(x,y-1,z,1),U->Get(x,y,z,2),UMyMzmpz); \
SUNcGroup::AdvancedOperations::DDUU(U->Get(x,y,z,2),U->Get(x-1,y,z,0),U->Get(x-1,y,z,2),U->Get(x-1,y,z+1,0),UMzMxmpz);

//OK

#define COMPUTE_MOVED_PLAQUETTES(x,y,z,U)\
\
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z,0),Uxy,U->Get(x,y,z,0),Dx_Uxy_Ux); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y-1,z,1),UxMy,U->Get(x,y-1,z,1),Uymy_UxMy_Dymy); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y-1,z,0),Uymy_UxMy_Dymy,U->Get(x,y-1,z,0),DxmyUymy_UxMy_DymyUxmy); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z,0),Uzx,U->Get(x,y,z,0),Dx_Uzx_Ux); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y,z-1,2),UMzx,U->Get(x,y,z-1,2),Uzmz_UMzx_Dzmz); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z-1,0),Uzmz_UMzx_Dzmz,U->Get(x,y,z-1,0),DxmzUzmz_UMzx_DzmzUxmz); \
\
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z,1),Uxy,U->Get(x,y,z,1),Dy_Uxy_Uy); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x-1,y,z,0),UMxy,U->Get(x-1,y,z,0),Uxmx_UMxy_Dxmx); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x-1,y,z,1),Uxmx_UMxy_Dxmx,U->Get(x-1,y,z,1),DymxUxmx_UMxy_DxmxUymx); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z,1),Uyz,U->Get(x,y,z,1),Dy_Uyz_Uy); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y,z-1,2),UyMz,U->Get(x,y,z-1,2),Uzmz_UyMz_Dzmz); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z-1,1),Uzmz_UyMz_Dzmz,U->Get(x,y,z-1,1),DymzUzmz_UyMz_DzmzUymz); \
\
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z,2),Uzx,U->Get(x,y,z,2),Dz_Uzx_Uz); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x-1,y,z,0),UzMx,U->Get(x-1,y,z,0),Uxmx_UzMx_Dxmx); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x-1,y,z,2),Uxmx_UzMx_Dxmx,U->Get(x-1,y,z,2),DzmxUxmx_UzMx_DxmxUzmx); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z,2),Uyz,U->Get(x,y,z,2),Dz_Uyz_Uz); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y-1,z,1),UMyz,U->Get(x,y-1,z,1),Uymy_UMyz_Dymy); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y-1,z,2),Uymy_UMyz_Dymy,U->Get(x,y-1,z,2),DzmyUymy_UMyz_DymyUzmy);

//HERE I WILL ATTEMPT TO CONSTRUCT ALL THE NEW PLAQUETTES WE NEED
#define SET_SECOND_PWR_PLAQUETTE_BUFFERS() \
\
SU_Nc_FUNDAMENTAL_FORMAT Uxympxmy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uxympymx[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Uxymmy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uxymmx[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Uzxmpxmz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uzxmpzmx[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Uzxmmz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uzxmmx[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Uyzmpymz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uyzmpzmy[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Uyzmmz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uyzmmy[SUNcGroup::MatrixSize]; \
\
DOUBLE ReTrIDMinusUxympx,ReTrIDMinusUxym,ReTrIDMinusUxympxmy,ReTrIDMinusUxymmy; \
DOUBLE ReTrIDMinusUzxmpx,ReTrIDMinusUzxm,ReTrIDMinusUzxmpxmz,ReTrIDMinusUzxmmz; \
\
DOUBLE ReTrIDMinusUxympy,ReTrIDMinusUxympymx,ReTrIDMinusUxymmx; \
DOUBLE ReTrIDMinusUyzmpy,ReTrIDMinusUyzm,ReTrIDMinusUyzmpymz,ReTrIDMinusUyzmmz; \
\
DOUBLE ReTrIDMinusUzxmpz,ReTrIDMinusUzxmpzmx,ReTrIDMinusUzxmmx; \
DOUBLE ReTrIDMinusUyzmpz,ReTrIDMinusUyzmpzmy,ReTrIDMinusUyzmmy;


#define COMP_SECOND_PWR_U_BUFFERS(x,y,z,U) \
ReTrIDMinusUxympx=SUNcGroup::Operations::ReTrIDMinusU(Uxympx); \
ReTrIDMinusUxym=SUNcGroup::Operations::ReTrIDMinusU(Uxy); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x+1,y-1,z,0),U->Get(x+2,y-1,z,1),U->Get(x+1,y,z,0),U->Get(x+1,y-1,z,1),Uxympxmy); \
ReTrIDMinusUxympxmy=SUNcGroup::Operations::ReTrIDMinusU(Uxympxmy); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y-1,z,0),U->Get(x+1,y-1,z,1),U->Get(x,y,z,0),U->Get(x,y-1,z,1),Uxymmy); \
ReTrIDMinusUxymmy=SUNcGroup::Operations::ReTrIDMinusU(Uxymmy); \
\
ReTrIDMinusUzxmpx=SUNcGroup::Operations::ReTrIDMinusU(Uzxmpx); \
ReTrIDMinusUzxm=SUNcGroup::Operations::ReTrIDMinusU(Uzx); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x+1,y,z-1,2),U->Get(x+1,y,z,0),U->Get(x+2,y,z-1,2),U->Get(x+1,y,z-1,0),Uzxmpxmz); \
ReTrIDMinusUzxmpxmz=SUNcGroup::Operations::ReTrIDMinusU(Uzxmpxmz); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y,z-1,2),U->Get(x,y,z,0),U->Get(x+1,y,z-1,2),U->Get(x,y,z-1,0),Uzxmmz); \
ReTrIDMinusUzxmmz=SUNcGroup::Operations::ReTrIDMinusU(Uzxmmz); \
\
ReTrIDMinusUxympy=SUNcGroup::Operations::ReTrIDMinusU(Uxympy); \
ReTrIDMinusUxym=SUNcGroup::Operations::ReTrIDMinusU(Uxy); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x-1,y+1,z,0),U->Get(x,y+1,z,1),U->Get(x-1,y+2,z,0),U->Get(x-1,y+1,z,1),Uxympymx); \
ReTrIDMinusUxympymx=SUNcGroup::Operations::ReTrIDMinusU(Uxympymx); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x-1,y,z,0),U->Get(x,y,z,1),U->Get(x-1,y+1,z,0),U->Get(x-1,y,z,1),Uxymmx); \
ReTrIDMinusUxymmx=SUNcGroup::Operations::ReTrIDMinusU(Uxymmx); \
\
ReTrIDMinusUyzmpy=SUNcGroup::Operations::ReTrIDMinusU(Uyzmpy); \
ReTrIDMinusUyzm=SUNcGroup::Operations::ReTrIDMinusU(Uyz); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y+1,z-1,1),U->Get(x,y+2,z-1,2),U->Get(x,y+1,z,1),U->Get(x,y+1,z-1,2),Uyzmpymz); \
ReTrIDMinusUyzmpymz=SUNcGroup::Operations::ReTrIDMinusU(Uyzmpymz); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y,z-1,1),U->Get(x,y+1,z-1,2),U->Get(x,y,z,1),U->Get(x,y,z-1,2),Uyzmmz); \
ReTrIDMinusUyzmmz=SUNcGroup::Operations::ReTrIDMinusU(Uyzmmz); \
\
SUNcGroup::AdvancedOperations::UUDD(U->Get(x-1,y,z,2),U->Get(x-1,y,z+1,0),U->Get(x,y,z,2),U->Get(x-1,y,z,0),Uzxmmx); \
ReTrIDMinusUzxmmx=SUNcGroup::Operations::ReTrIDMinusU(Uzxmmx); \
SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y-1,z,1),U->Get(x,y,z,2),U->Get(x,y-1,z+1,1),U->Get(x,y-1,z,2),Uyzmmy); \
ReTrIDMinusUyzmmy=SUNcGroup::Operations::ReTrIDMinusU(Uyzmmy); \
\
SUNcGroup::AdvancedOperations::UUDD(U->Get(x-1,y,z,2),U->Get(x-1,y,z+1,0),U->Get(x,y,z,2),U->Get(x-1,y,z,0),Uzxmmx); \
ReTrIDMinusUzxmmx=SUNcGroup::Operations::ReTrIDMinusU(Uzxmmx); \
\
if(z < Lattice::N[2] - 2) { \
    ReTrIDMinusUzxmpz=SUNcGroup::Operations::ReTrIDMinusU(Uzxmpz); \
    SUNcGroup::AdvancedOperations::UUDD(U->Get(x-1,y,z+1,2),U->Get(x-1,y,z+2,0),U->Get(x,y,z+1,2),U->Get(x-1,y,z+1,0),Uzxmpzmx); \
    ReTrIDMinusUzxmpzmx=SUNcGroup::Operations::ReTrIDMinusU(Uzxmpzmx); \
\
    ReTrIDMinusUyzmpz=SUNcGroup::Operations::ReTrIDMinusU(Uyzmpz); \
    SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y-1,z+1,1),U->Get(x,y,z+1,2),U->Get(x,y-1,z+2,1),U->Get(x,y-1,z+1,2),Uyzmpzmy); \
    ReTrIDMinusUyzmpzmy=SUNcGroup::Operations::ReTrIDMinusU(Uyzmpzmy); \
} else { \
    ReTrIDMinusUzxmpz=SUNcGroup::Operations::ReTrIDMinusU(Uzxmpz); \
    SUNcGroup::AdvancedOperations::UUDD(U->Get(x-1,y,z,2),U->Get(x-1,y,z+1,0),U->Get(x,y,z,2),U->Get(x-1,y,z,0),Uzxmpzmx); \
    ReTrIDMinusUzxmpzmx=SUNcGroup::Operations::ReTrIDMinusU(Uzxmpzmx); \
    \
    ReTrIDMinusUyzmpz=SUNcGroup::Operations::ReTrIDMinusU(Uyzmpz); \
    SUNcGroup::AdvancedOperations::UUDD(U->Get(x,y-1,z,1),U->Get(x,y,z,2),U->Get(x,y-1,z+1,1),U->Get(x,y-1,z,2),Uyzmpzmy); \
    ReTrIDMinusUyzmpzmy=SUNcGroup::Operations::ReTrIDMinusU(Uyzmpzmy); \
}
//Above If-Else statement used to avoid Segmentation fault with GhostCells at endpoint of lattice in z-dir.

//COMPUTATION OF THE PLAQUETTE TRACES
#define COMPUTE_ELEMENTARY_COLOR_TRACES() \
\
SUNcGroup::Operations::ReTrIGenU(Uxy,ReTrITaUxy); \
SUNcGroup::Operations::ReTrIGenU(Uyz,ReTrITaUyz); \
SUNcGroup::Operations::ReTrIGenU(Uzx,ReTrITaUzx);


#define COMPUTE_NEIGHBORING_COLOR_TRACES() \
\
SUNcGroup::Operations::ReTrIGenU(UMxy,ReTrITaUMxy); \
SUNcGroup::Operations::ReTrIGenU(UMyz,ReTrITaUMyz); \
SUNcGroup::Operations::ReTrIGenU(UMzx,ReTrITaUMzx); \
\
SUNcGroup::Operations::ReTrIGenU(UxMy,ReTrITaUxMy); \
SUNcGroup::Operations::ReTrIGenU(UyMz,ReTrITaUyMz); \
SUNcGroup::Operations::ReTrIGenU(UzMx,ReTrITaUzMx);

#define COMPUTE_EXTENDED_COLOR_TRACES() \
\
SUNcGroup::Operations::ReTrIGenU(UMxMy,ReTrITaUMxMy); \
SUNcGroup::Operations::ReTrIGenU(UMyMz,ReTrITaUMyMz); \
SUNcGroup::Operations::ReTrIGenU(UMzMx,ReTrITaUMzMx);


#define COMPUTE_MOVED_COLOR_TRACES()\
\
SUNcGroup::Operations::ReTrIGenU(Dx_Uxy_Ux,ReTrITaDx_Uxy_Ux); \
SUNcGroup::Operations::ReTrIGenU(Uymy_UxMy_Dymy,ReTrITaUymy_UxMy_Dymy); \
SUNcGroup::Operations::ReTrIGenU(DxmyUymy_UxMy_DymyUxmy,ReTrITaDxmyUymy_UxMy_DymyUxmy); \
SUNcGroup::Operations::ReTrIGenU(Dx_Uzx_Ux,ReTrITaDx_Uzx_Ux); \
SUNcGroup::Operations::ReTrIGenU(Uzmz_UMzx_Dzmz,ReTrITaUzmz_UMzx_Dzmz); \
SUNcGroup::Operations::ReTrIGenU(DxmzUzmz_UMzx_DzmzUxmz,ReTrITaDxmzUzmz_UMzx_DzmzUxmz); \
\
SUNcGroup::Operations::ReTrIGenU(Dy_Uxy_Uy,ReTrITaDy_Uxy_Uy); \
SUNcGroup::Operations::ReTrIGenU(Uxmx_UMxy_Dxmx,ReTrITaUxmx_UMxy_Dxmx); \
SUNcGroup::Operations::ReTrIGenU(DymxUxmx_UMxy_DxmxUymx,ReTrITaDymxUxmx_UMxy_DxmxUymx); \
SUNcGroup::Operations::ReTrIGenU(Dy_Uyz_Uy,ReTrITaDy_Uyz_Uy); \
SUNcGroup::Operations::ReTrIGenU(Uzmz_UyMz_Dzmz,ReTrITaUzmz_UyMz_Dzmz); \
SUNcGroup::Operations::ReTrIGenU(DymzUzmz_UyMz_DzmzUymz,ReTrITaDymzUzmz_UyMz_DzmzUymz); \
\
SUNcGroup::Operations::ReTrIGenU(Dz_Uzx_Uz,ReTrITaDz_Uzx_Uz); \
SUNcGroup::Operations::ReTrIGenU(Uxmx_UzMx_Dxmx,ReTrITaUxmx_UzMx_Dxmx); \
SUNcGroup::Operations::ReTrIGenU(DzmxUxmx_UzMx_DxmxUzmx,ReTrITaDzmxUxmx_UzMx_DxmxUzmx); \
SUNcGroup::Operations::ReTrIGenU(Dz_Uyz_Uz,ReTrITaDz_Uyz_Uz); \
SUNcGroup::Operations::ReTrIGenU(Uymy_UMyz_Dymy,ReTrITaUymy_UMyz_Dymy); \
SUNcGroup::Operations::ReTrIGenU(DzmyUymy_UMyz_DymyUzmy,ReTrITaDzmyUymy_UMyz_DymyUzmy); 


#endif
