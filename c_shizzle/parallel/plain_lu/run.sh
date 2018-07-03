cd /home/sameer/gitrepos/scalapack-2.0.2
make clean
make lib -j100
cd /home/sameer/gitrepos/scratch/c_shizzle/parallel/plain_lu
make -k plain_lu
echo "input files...\n"
cat 0input.txt
cat 1input.txt
cat 2input.txt
cat 3input.txt

echo "outputs...\n"
cat 0.txt
cat 1.txt
cat 2.txt
cat 3.txt
