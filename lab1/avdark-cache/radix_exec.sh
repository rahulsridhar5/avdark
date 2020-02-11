#Executes the radix codes

./pin-avdc.sh -s 16384 -l 32 -a 1 -o radix1.out -- ../radix/radix -n 100000 
./pin-avdc.sh -s 16384 -l 32 -a 2 -o radix2.out -- ../radix/radix -n 100000 
./pin-avdc.sh -s 32768 -l 16 -a 1 -o radix3.out -- ../radix/radix -n 100000 
./pin-avdc.sh -s 32768 -l 32 -a 1 -o radix4.out -- ../radix/radix -n 100000 
./pin-avdc.sh -s 32768 -l 64 -a 1 -o radix5.out -- ../radix/radix -n 100000 
./pin-avdc.sh -s 32768 -l 16 -a 2 -o radix6.out -- ../radix/radix -n 100000 
./pin-avdc.sh -s 32768 -l 32 -a 2 -o radix7.out -- ../radix/radix -n 100000 
./pin-avdc.sh -s 32768 -l 64 -a 2 -o radix8.out -- ../radix/radix -n 100000
./pin-avdc.sh -s 65536 -l 32 -a 1 -o radix9.out -- ../radix/radix -n 100000 
./pin-avdc.sh -s 65536 -l 32 -a 2 -o radix10.out -- ../radix/radix -n 100000 