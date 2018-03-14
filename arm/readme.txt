/*
 * Author: Joost Renes
 * Version: 2017-05-23
 * Public Domain
 */

/* Compilation: from http://munacl.cryptojedi.org/curve25519-cortexm0.shtml */

apt-get install llvm clang gcc-arm-none-eabi
apt-get install libc6-dev-i386 
apt-get install libusb-1.0-0-dev

git clone https://github.com/texane/stlink.git
cd stlink
./autogen.sh
./configure
make && make install 

3.3V → 3.3V
TXD → PA3
RXD → PA2
GND → GND
5V → DO NOT CONNECT

make

/* For testing: */

test/test_stm32f0.sh 
test/stack_stm32f0.sh
test/speed_stm32f0.sh
