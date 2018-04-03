CXX = mpicxx -ggdb3 -Wall -O3 -fopenmp -lm -I.
CFLAGS=-I.
SOURCES = libscalapack.a

.cpp.o:
	$(CXX) -c $? -o $@

blacs: blacs_example.o $(SOURCES)
	$(CXX) $?
	mpirun -n 9 ./a.out

pblas: pblas_matmul.o $(SOURCES)
	$(CXX) $?
	mpirun -n 4 ./a.out

clean:
	rm -f *.o *.out
