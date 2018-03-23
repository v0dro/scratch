CXX = mpicxx -ggdb3 -Wall -O3 -fopenmp -I.
CFLAGS=-I.
SOURCES = libscalapack.a

.cpp.o:
	$(CXX) -c $? -o $@

blacs: blacs_example.o $(SOURCES)
	$(CXX) $?
	./a.out
clean:
	rm -f *.o *.out
