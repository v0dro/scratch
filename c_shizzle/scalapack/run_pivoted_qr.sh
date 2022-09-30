#!/bin/bash
mpicxx -L/opt/homebrew/Cellar/scalapack/2.2.0_1/lib -lscalapack pivoted_qr.cpp
./a.out
