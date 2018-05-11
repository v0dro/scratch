CXX = g++ -ggdb3 -Wall -fopenmp -lm -I.
CFLAGS=-I.
#SOURCES = libscalapack.a	

.cpp.o:
	$(CXX) -c $? -o $@

main: main.o $(SOURCES)
	$(CXX) $?
	./a.out 20

perf: main.o $(SOURCES)
	$(CXX) $? -lblas
	perf stat -B ./a.out 2000

clean:
	rm -f *.o *.out
