.global fe1271_freeze
.type fe1271_freeze, @function
 
 //*****************************************************
 // fe1271_freeze
 // reduces the top bit of the 128 bit integer
 //
 // Inputs:
 //     r       in register R25:R24
 //     t       in register R23:R22
 //

 fe1271_freeze:
   
   MOVW R26, R22 ; integer in X 
   MOVW R30, R24 ; result in Z

   CLR R19

   ADIW R26, 15 ; ptr to byte 15
   LD R18, X
   SBIW R26, 15 ; restore ptr to byte 0

   LSL R18 ; rotate highest bit into carry bit
   ROL R19 ; save carry
   LSR R18 ; save in R18 for carrying later

   LD R21, X+ ; load byte 0
   ADD R21, R19 ; add carry - save in R21 for possible extra carry bit

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+1, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+2, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+3, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+4, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+5, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+6, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+7, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+8, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+9, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+10, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+11, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+12, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+13, R20 ; store back

   LD R20, X+ ; load byte 1
   ADC R20, R1 ; add carry
   STD Z+14, R20 ; store back

   ; X+15 was stored in R18
   ADC R18, R1 ; add carry

   ; possible highest bit still set
   ; carry that again
   LSL R18
   ADC R21, R1
   LSR R18

   STD Z+15, R18 ; store back
   STD Z+0, R21

   RET
