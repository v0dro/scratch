COMPILER = gfortran
SOURCES = ~/gitrepos/lapack-3.8.0/libtmglib.a ~/gitrepos/lapack-3.8.0/liblapack.a ~/gitrepos/lapack-3.8.0/librefblas.a

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
