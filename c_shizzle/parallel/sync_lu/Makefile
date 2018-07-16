CXX = mpicxx -g -DUSE_BLAS -fPIC -std=c++11 -O0 -fopenmp -lm -I.
CFLAGS=-I.
SOURCES = utils.o implementation.o ~/gitrepos/scalapack-2.0.2/libscalapack.a

.PHONY: clean

.cpp.o:
	$(CXX) -c $? -o $@

sync_lu: sync_lu_decomp.o $(SOURCES)
	$(CXX) $? -lgfortran
	mpirun -n 4 ./a.out

clean:
	rm -f *.o *.out
