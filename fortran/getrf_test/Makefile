COMPILER = gfortran
SOURCES = libtmglib.a liblapack.a librefblas.a

.PHONY: clean

.f90.o:
	$(COMPILER) -c $? -o $@

getrf_test.o: getrf_test.f90
	$(COMPILER) -c getrf_test.f90

getrf_test: getrf_test.o $(SOURCES) 
	$(COMPILER) $?
	./a.out

clean:
	rm -f *.o *.out
