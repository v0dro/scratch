CXX = mpicxx -ggdb3 -Wall -fPIC -O3 -fopenmp -lm -I.
CFLAGS=-I.
SOURCES = utils.o libscalapack.a

.PHONY: clean

.cpp.o:
	$(CXX) -c $? -o $@

sync_lu: sync_lu_decomp.o $(SOURCES)
	$(CXX) $? -lblas -lgfortran
	mpirun -n 4 ./a.out

clean:
	rm -f *.o *.out
