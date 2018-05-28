CXX = g++ -ggdb3 -mavx2 -mfma -Wall -lm -I.
CFLAGS=-I.
#SOURCES = libscalapack.a	

.PHONY : clean

.cpp.o:
	$(CXX) -c $? -o $@

main: main.o $(SOURCES)
	$(CXX) $?
	./a.out 1024

b_pack: b_pack.o $(SOURCES)
	$(CXX) $?
	./a.out

perf: main.o $(SOURCES)
	$(CXX) $? -lblas
	perf stat -B ./a.out 2000

clean:
	rm -f *.o *.out
