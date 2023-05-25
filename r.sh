#/bin/bash

rm *.o
rm *.out

echo "Compile dateandtime.c"
gcc -c -Wall -no-pie -o shopping.o shopping.c -std=c17

echo "Link the object files"
gcc -m64 -no-pie -o parse.out shopping.o -std=c17

chmod u+x parse.out

echo "Run the program"
./parse.out

echo "The script file will terminate"

