CXX = mpicxx -std=c++11 -ggdb3 -Wall -fPIC -O3 -fopenmp -lm -I.
CFLAGS=-I.
SOURCES = libscalapack.a

.cpp.o:
	$(CXX) -c $? -o $@

plain_lu: scalapack_lu_decomp.o $(SOURCES)
	$(CXX) $? -lblas -lgfortran
	mpirun -n 4 ./a.out

clean:
	rm -f *.o *.out
