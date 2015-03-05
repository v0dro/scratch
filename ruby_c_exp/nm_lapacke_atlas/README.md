This gem uses ATLAS installed on the user's computer thru the debian repositories.

The biggest problem with using LAPACKE from netlib is that linking the generated object files with a program requires a FORTRAN linker.

Here's a line from the lapacke Makefile:

```

LIBRAIRIES= ../../$(LAPACKELIB) ../../$(LAPACKLIB) $(BLASLIB) 

# Double Precision Examples
xexample_DGESV_rowmajor: example_DGESV_rowmajor.o lapacke_example_aux.o $(LIBRAIRIES)
    $(LOADER) $(LOADOPTS)  example_DGESV_rowmajor.o lapacke_example_aux.o \
    $(LIBRAIRIES) -o $@
    ./$@
```

`LOADER` is defined as `gfortran`. GCC will fail in this case.