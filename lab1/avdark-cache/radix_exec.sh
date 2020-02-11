#Executes the radix codes

./pin-avdc.sh -s 16384 -l 32 -a 1 -- ../radix/radix -n 100000 -o radix1.out
./pin-avdc.sh -s 16384 -l 32 -a 2 -- ../radix/radix -n 100000 -o radix2.out
./pin-avdc.sh -s 32768 -l 16 -a 1 -- ../radix/radix -n 100000 -o radix3.out
./pin-avdc.sh -s 32768 -l 32 -a 1 -- ../radix/radix -n 100000 -o radix4.out
./pin-avdc.sh -s 32768 -l 64 -a 1 -- ../radix/radix -n 100000 -o radix5.out
./pin-avdc.sh -s 32768 -l 16 -a 2 -- ../radix/radix -n 100000 -o radix6.out
./pin-avdc.sh -s 32768 -l 32 -a 2 -- ../radix/radix -n 100000 -o radix7.out
./pin-avdc.sh -s 32768 -l 64 -a 2 -- ../radix/radix -n 100000 -o radix8.outv
./pin-avdc.sh -s 65536 -l 32 -a 1 -- ../radix/radix -n 100000 -o radix9.out
./pin-avdc.sh -s 65536 -l 32 -a 2 -- ../radix/radix -n 100000 -o radix10.out