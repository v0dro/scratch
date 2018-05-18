CXX = mpicxx -ggdb3 -Wall -fPIC -std=c++11 -O3 -fopenmp -lm -I.
CFLAGS=-I.
SOURCES = utils.o implementation.o libscalapack.a

.PHONY: clean

.cpp.o:
	$(CXX) -c $? -o $@

sync_lu: sync_lu_decomp.o $(SOURCES)
	$(CXX) $? -lblas -lgfortran
	mpirun -n 1 ./a.out
	bash show.sh

clean:
	rm -f *.o *.out
