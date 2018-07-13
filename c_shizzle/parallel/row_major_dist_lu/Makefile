CXX_TSUBAME = mpicxx -g -Wall -fPIC -DUSE_BLAS -std=c++11 -O3 -fopenmp -I.
CXX = mpicxx -g -Wall -fPIC -std=c++11 -O3 -fopenmp -lm -I.
CFLAGS=-I.
SOURCES = utils.o implementation.o unit_tests.o libscalapack.a

.PHONY: clean

.cpp.o:
	$(CXX) -c $? -o $@

sync_lu_tsubame: sync_lu_decomp.o $(SOURCES)
	$(CXX_TSUBAME) $? -lgfortran
	ddt a.out

sync_lu: sync_lu_decomp.o $(SOURCES)
	$(CXX) $? -lblas -lgfortran
	mpirun -np 4 xterm -e gdb ./a.out
#	mpirun -np 4 ./a.out

clean:
	rm -f *.o *.out
