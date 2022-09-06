clang a.c -o a.o -c
clang++ app.cpp -o app.o -c
clang++ -o a.out app.o a.o
./a.out 12

# clang++ a1.cpp -o a1.o -c
# clang++ app.cpp -o app.o -c
# clang++ -o a.out a1.o app.o
# ./a.out 12
