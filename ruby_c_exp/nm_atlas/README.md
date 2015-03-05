# Ruby interface to CLAPACK compiled with ATLAS

Since I found ATLAS quite complex to link with CLAPACK, I thought it did not make sense to include the source files with the gem (like I've done for openBLAS).

Instead, for this sample gem I've linked with the exact same binaries that NMatrix links with. This is for demonstrating the benchmarks of the gesv functions, as a comparison between different implementations of the BLAS interface.

The binaries that I've linked against are those from the Debian sources.

This gem and the functions it uses is most reminiscent to nmatrix in its present form.

An advantage that the LAPACK interface of ATLAS would have over the vanilla F2C'd CLAPACK interface that I've implemented in CLAPACK/openBLAS binding is the ability to specify whether the supplied matrices are in row major or column major. Again as I've said in the proposal, I think this can be overcome with an optimized transpose function.

The benchmarks turn out to be the following:
```

    user     system      total        real
    0.130000   0.000000   0.130000 (  0.130056)
```

This is even faster than what was reported with LAPACKE and openBLAS!

But again, ease of installation must be considered. Bundling openBLAS source with the gem would be great because installation is trivial in the case of openBLAS, unlike ATLAS.

We can let users choose ATLAS as the underlying blas implementation with ENV variables. An advantage in this regard is that CLAPACK compiles with ATLAS easily.