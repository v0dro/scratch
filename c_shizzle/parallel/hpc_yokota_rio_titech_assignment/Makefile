CXX = g++ -O0 -fopenmp -mavx2 -mfma -Wall -lm -I.
CFLAGS=-I.

.PHONY : clean

.cpp.o:
	$(CXX) -c $? -o $@

main: main.o $(SOURCES)
	$(CXX) $?
	./a.out 1024

clean:
	rm -f *.o *.out
