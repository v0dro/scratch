CXX = g++ -ggdb3 -Wall -fopenmp -lm -I.
CFLAGS=-I.
#SOURCES = libscalapack.a	

.cpp.o:
	$(CXX) -c $? -o $@

main: main.o $(SOURCES)
	$(CXX) $? -lblas
	./a.out 1000

clean:
	rm -f *.o *.out
