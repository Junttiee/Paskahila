all:
	  mpicxx -Wall -Wno-delete-non-virtual-dtor -Wno-long-long -Wno-unused-variable -pedantic-errors -o CollSUNcSolver.exe SourceCode/SUNcSolver.cpp ${GSL} -I${FFTW_INC} -L${FFTW_DIR} -lstdc++ -lgsl -lgslcblas -fopenmp -lfftw3_omp -lfftw3
