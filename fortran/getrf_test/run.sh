cd ~/gitrepos/lapack-3.8.0
#make clean
make -j100 lib cblaslib blaslib
cd ~/gitrepos/scratch/fortran/getrf_test
make getrf_test
