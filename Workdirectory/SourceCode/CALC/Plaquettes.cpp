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
/*Below we use the existing elementary plaquettes, even if the naming is confusing.
So essentially if we need Uyx we mark it as Uxy as that one exists and can be used in computations.
Moreover because (DxUyxUx)^}^\dag = Dx_Uxy_Ux we can and must use Uyx and then *(-1) ReTr expression will give us the correct ReTr of Dx_Uxy_Ux :) */

/* There could be a way to have some loops that go through j XOR k to implement all the terms that differ by just link ect, not sure if possible of worth it. */


#define SET_MOVED_PLAQUETTE_BUFFERS() \
\
SU_Nc_FUNDAMENTAL_FORMAT Ux_Uxy_Dx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Dy_UxMy_Uy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UxDy_UxMy_UyDx[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Dx_Uzx_Ux[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uz_UMzx_Dz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT DxUz_UMzx_DzUx[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Dy_Uxy_Uy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Ux_UMxy_Dx[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT DyUx_UMxy_DxUy[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Uy_Uyz_Dy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Dz_UyMz_Uz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UyDz_UyMz_UzDy[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Uz_Uzx_Dz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Dx_UzMx_Ux[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT UzDx_UzMx_UxDz[SUNcGroup::MatrixSize]; \
\
SU_Nc_FUNDAMENTAL_FORMAT Dz_Uyz_Uz[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT Uy_UMyz_Dy[SUNcGroup::MatrixSize]; \
SU_Nc_FUNDAMENTAL_FORMAT DzUy_UMyz_DyUz[SUNcGroup::MatrixSize]; 

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
DOUBLE ReTrITaUx_Uxy_Dx[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDy_UxMy_Uy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUxDy_UxMy_UyDx[SUNcAlgebra::NumberOfGenerators]; \
\
DOUBLE ReTrITaDx_Uzx_Ux[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUz_UMzx_Dz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDxUz_UMzx_DzUx[SUNcAlgebra::NumberOfGenerators]; \
\
DOUBLE ReTrITaDy_Uxy_Uy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUx_UMxy_Dx[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDyUx_UMxy_DxUy[SUNcAlgebra::NumberOfGenerators]; \
\
DOUBLE ReTrITaUy_Uyz_Dy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDz_UyMz_Uz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUyDz_UyMz_UzDy[SUNcAlgebra::NumberOfGenerators]; \
\
DOUBLE ReTrITaUz_Uzx_Dz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDx_UzMx_Ux[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUzDx_UzMx_UxDz[SUNcAlgebra::NumberOfGenerators]; \
\
DOUBLE ReTrITaDz_Uyz_Uz[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaUy_UMyz_Dy[SUNcAlgebra::NumberOfGenerators]; \
DOUBLE ReTrITaDzUy_UMyz_DyUz[SUNcAlgebra::NumberOfGenerators]; 

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
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y,z,0),Uxy,U->Get(x,y,z,0),Ux_Uxy_Dx); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y-1,z,1),UxMy,U->Get(x,y-1,z,1),Dy_UxMy_Uy); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y-1,z,0),Dy_UxMy_Uy,U->Get(x,y-1,z,0),UxDy_UxMy_UyDx); \
\
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z,0),Uzx,U->Get(x,y,z,0),Dx_Uzx_Ux); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y,z-1,2),UMzx,U->Get(x,y,z-1,2),Uz_UMzx_Dz); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z-1,0),Uz_UMzx_Dz,U->Get(x,y,z-1,0),DxUz_UMzx_DzUx); \
\
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z,1),Uxy,U->Get(x,y,z,1),Dy_Uxy_Uy); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x-1,y,z,0),UMxy,U->Get(x-1,y,z,0),Ux_UMxy_Dx); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x-1,y,z,1),Ux_UMxy_Dx,U->Get(x-1,y,z,1),DyUx_UMxy_DxUy); \
\
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y,z,1),Uyz,U->Get(x,y,z,1),Uy_Uyz_Dy); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z-1,2),UyMz,U->Get(x,y,z-1,2),Dz_UyMz_Uz); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y,z-1,1),Dz_UyMz_Uz,U->Get(x,y,z-1,1),UyDz_UyMz_UzDy); \
\
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y,z,2),Uzx,U->Get(x,y,z,2),Uz_Uzx_Dz); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x-1,y,z,0),UzMx,U->Get(x-1,y,z,0),Dx_UzMx_Ux); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x-1,y,z,2),Dx_UzMx_Ux,U->Get(x-1,y,z,2),UzDx_UzMx_UxDz); \
\
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y,z,2),Uyz,U->Get(x,y,z,2),Dz_Uyz_Uz); \
SUNcGroup::AdvancedOperations::UUD(U->Get(x,y-1,z,1),UMyz,U->Get(x,y-1,z,1),Uy_UMyz_Dy); \
SUNcGroup::AdvancedOperations::DUU(U->Get(x,y-1,z,2),Uy_UMyz_Dy,U->Get(x,y-1,z,2),DzUy_UMyz_DyUz);

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
SUNcGroup::Operations::ReTrIGenU(Ux_Uxy_Dx,ReTrITaUx_Uxy_Dx); \
SUNcGroup::Operations::ReTrIGenU(Dy_UxMy_Uy,ReTrITaDy_UxMy_Uy); \
SUNcGroup::Operations::ReTrIGenU(UxDy_UxMy_UyDx,ReTrITaUxDy_UxMy_UyDx); \
\
SUNcGroup::Operations::ReTrIGenU(Dx_Uzx_Ux,ReTrITaDx_Uzx_Ux); \
SUNcGroup::Operations::ReTrIGenU(Uz_UMzx_Dz,ReTrITaUz_UMzx_Dz); \
SUNcGroup::Operations::ReTrIGenU(DxUz_UMzx_DzUx,ReTrITaDxUz_UMzx_DzUx); \
\
SUNcGroup::Operations::ReTrIGenU(Dy_Uxy_Uy,ReTrITaDy_Uxy_Uy); \
SUNcGroup::Operations::ReTrIGenU(Ux_UMxy_Dx,ReTrITaUx_UMxy_Dx); \
SUNcGroup::Operations::ReTrIGenU(DyUx_UMxy_DxUy,ReTrITaDyUx_UMxy_DxUy); \
\
SUNcGroup::Operations::ReTrIGenU(Uy_Uyz_Dy,ReTrITaUy_Uyz_Dy); \
SUNcGroup::Operations::ReTrIGenU(Dz_UyMz_Uz,ReTrITaDz_UyMz_Uz); \
SUNcGroup::Operations::ReTrIGenU(UyDz_UyMz_UzDy,ReTrITaUyDz_UyMz_UzDy); \
\
SUNcGroup::Operations::ReTrIGenU(Uz_Uzx_Dz,ReTrITaUz_Uzx_Dz); \
SUNcGroup::Operations::ReTrIGenU(Dx_UzMx_Ux,ReTrITaDx_UzMx_Ux); \
SUNcGroup::Operations::ReTrIGenU(UzDx_UzMx_UxDz,ReTrITaUzDx_UzMx_UxDz); \
\
SUNcGroup::Operations::ReTrIGenU(Dz_Uyz_Uz,ReTrITaDz_Uyz_Uz); \
SUNcGroup::Operations::ReTrIGenU(Uy_UMyz_Dy,ReTrITaUy_UMyz_Dy); \
SUNcGroup::Operations::ReTrIGenU(DzUy_UMyz_DyUz,ReTrITaDzUy_UMyz_DyUz); 


#endif
