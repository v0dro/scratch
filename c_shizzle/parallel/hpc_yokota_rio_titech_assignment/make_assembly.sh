g++ -S -fverbose-asm -mavx2 -mfma -g -O0 main.cpp -o main_o0.s
as -alhnd main_o0.s > main_o0.lst
