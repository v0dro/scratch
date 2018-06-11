CXX = g++ -O0 -fopenmp -mavx2 -mfma -Wall -lm -I.
CFLAGS=-I.

.PHONY : clean

.cpp.o:
	$(CXX) -c $? -o $@

main: main.o $(SOURCES)
	$(CXX) $?
	./a.out 128

clean:
	rm -f *.o *.out
