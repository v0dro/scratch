# Speed/usability comparison between ruby wrappers to various fast linear algebra libraries

There are three folders `nm_clapack`, `nm_lapacke`, `nm_atlas` and `nm_lapacke_atlas` each of which contain little ruby libraries that interface with different C implementations of LAPACK and BLAS.

Heres a brief of each gem:

* **nm\_clapack** - A small ruby gem bindings with CLAPACK and using openBLAS as the underlying BLAS implementation. This is the slowest. Compiled from source on my machine and then imported the binaries into the gem.
* **nm\_lapacke** - A small gem with bindings for LAPACKE and using openBLAS as the the underlying BLAS implementation. Compiled LAPACKE against openBLAS on my machine and then imported the binaries.
* **nm\_atlas** -  A small gem with bindings for CLAPACK and ATLAS obtained off the `libatlas-base-dev` package from the Debian repositories. This is what is currently used in nmatrix.
* **nm\_lapacke\_atlas** - A small gem with bindings for LAPACKE compiled against the ATLAS packages from `libatlas-base-dev`. LAPACKE compiled manually against supplied ATLAS binaries.


Refer to [this blog post](http://v0dro.github.io/blog/2015/03/11/interfacing-and-benchmarking-high-performance-linear-algebra-libraries-with-ruby/) for a detailed description of the approaches followed and the benchmarks each of them yielded, along with a brief summary of the pros and cons of each.