CXX = mpicxx -ggdb3 -Wall -fPIC -std=c++11 -fopenmp -lm -I.
CFLAGS=-I.
SOURCES = ../row_major_dist_lu/libscalapack.a

.PHONY: clean

.cpp.o:
	$(CXX) -c $? -o $@

blacs_p2p_send_recv: blacs_p2p_send_recv.o $(SOURCES)
	$(CXX) $? -lblas -lgfortran
	mpirun -n 4 ./a.out

clean:
	rm -f *.o *.out
