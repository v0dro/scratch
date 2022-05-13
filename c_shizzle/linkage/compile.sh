# clang a.c -o a.o
# clang++ app.cpp -o app.o
# clang++ app.o a.o -o a.out
# ./a.out

clang++ a1.cpp -o a1.o -c
clang++ app.cpp -o app.o -c
clang++ -o a.out a1.o app.o
./a.out 12
