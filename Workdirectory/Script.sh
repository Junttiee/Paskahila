for i in `seq 1 8`
do
    mkdir -p SYMMETRIC/COLLISION/SEED${i}

    echo "#!/bin/bash -l" >> Coll${i}.sbat
    echo "" >>Coll${i}.sbat

    echo "#SBATCH --job-name=Sym${i}" >>Coll${i}.sbat
    echo "#SBATCH --account=lappi" >>Coll${i}.sbat
    echo "#SBATCH --partition=small" >>Coll${i}.sbat 
    echo "#SBATCH --time=26:00:00" >>Coll${i}.sbat
    echo "#SBATCH --nodes=1" >>Coll${i}.sbat
    echo "#SBATCH --cpus-per-task=40" >>Coll${i}.sbat
    echo "#SBATCH --mail-user=yannickdunlop@protonmail.com" >>Coll${i}.sbat
    echo "#SBATCH --mail-type=BEGIN,END,FAIL" >>Coll${i}.sbat

    echo "" >>Coll${i}.sbat

    echo "module add fftw/3.3.10-mpi-omp gsl" >>Coll${i}.sbat

    echo "" >>Coll${i}.sbat

    echo "./CollSUNcSolver.exe -Nx 128 -Ny 128 -Nz 1024 -ax 1.0 -ay 1.0 -az 0.25 -SEED ${i} -o SYMMETRIC/COLLISION/SEED${i}/" >>Coll${i}.sbat
    
    sbatch Coll${i}.sbat
done





for i in `seq 1 8`
do

    mkdir -p SYMMETRIC/LEFT_NUCLEUS/SEED${i}

    echo "#!/bin/bash -l" >> Left${i}.sbat
    echo "" >>Left${i}.sbat

    echo "#SBATCH --job-name=Left${i}" >>Left${i}.sbat
    echo "#SBATCH --account=lappi" >>Left${i}.sbat
    echo "#SBATCH --partition=small" >>Left${i}.sbat 
    echo "#SBATCH --time=26:00:00" >>Left${i}.sbat
    echo "#SBATCH --nodes=1" >>Left${i}.sbat
    echo "#SBATCH --cpus-per-task=40" >>Left${i}.sbat
    echo "#SBATCH --mail-user=yannickdunlop@protonmail.com" >>Left${i}.sbat
    echo "#SBATCH --mail-type=BEGIN,END,FAIL" >>Left${i}.sbat

    echo "" >>Left${i}.sbat

    echo "module add fftw/3.3.10-mpi-omp gsl" >>Left${i}.sbat

    echo "" >>Left${i}.sbat

    echo "./LeftSUNcSolver.exe -Nx 128 -Ny 128 -Nz 1024 -ax 1.0 -ay 1.0 -az 0.25 -SEED ${i} -o SYMMETRIC/LEFT_NUCLEUS/SEED${i}/" >>Left${i}.sbat

    sbatch Left${i}.sbat 
done





for i in `seq 1 8`
do

    mkdir -p SYMMETRIC/RIGHT_NUCLEUS/SEED${i}

    echo "#!/bin/bash -l" >> Right${i}.sbat
    echo "" >>Right${i}.sbat

    echo "#SBATCH --job-name=Right${i}" >>Right${i}.sbat
    echo "#SBATCH --account=lappi" >>Right${i}.sbat
    echo "#SBATCH --partition=small" >>Right${i}.sbat 
    echo "#SBATCH --time=26:00:00" >>Right${i}.sbat
    echo "#SBATCH --nodes=1" >>Right${i}.sbat
    echo "#SBATCH --cpus-per-task=40" >>Right${i}.sbat
    echo "#SBATCH --mail-user=yannickdunlop@protonmail.com" >>Right${i}.sbat
    echo "#SBATCH --mail-type=BEGIN,END,FAIL" >>Right${i}.sbat

    

    echo "" >>Right${i}.sbat

    echo "module add fftw/3.3.10-mpi-omp gsl" >>Right${i}.sbat

    echo "" >>Right${i}.sbat

    echo "./RightSUNcSolver.exe -Nx 128 -Ny 128 -Nz 1024 -ax 1.0 -ay 1.0 -az 0.25 -SEED ${i} -o SYMMETRIC/RIGHT_NUCLEUS/SEED${i}/" >>Right${i}.sbat
    
    sbatch Right${i}.sbat
done
