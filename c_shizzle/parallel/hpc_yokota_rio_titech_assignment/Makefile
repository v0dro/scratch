CXX = g++ -fopenmp -O0 -mavx2 -mfma -Wall -lm -I.
CFLAGS=-I.

.PHONY : clean

.cpp.o:
	$(CXX) -c $? -o $@

main: main.o $(SOURCES)
	$(CXX) $?

clean:
	rm -f *.o *.out
