/*
 * Author: Joost Renes
 * Version: 2017-05-23
 * Public Domain
 */

/* Compilation: from http://munacl.cryptojedi.org/curve25519-atmega.shtml */

apt-get update
apt-get install gcc-avr binutils-avr avr-libc avrdude

make

/* For testing: */

test/test_atmega2560.sh 
test/stack_atmega2560.sh
test/speed_atmega2560.sh
