.global bigint_red
.type bigint_red, @function

//*********************************************************
// bigint_red
// reduces the result modulo 2^127-1
//
// Inputs:
//    r       in register R25:R24
//    t       in register R23:R22
//

bigint_red:
  
  CLR R18           ; load a zero value
  
  MOVW R26, R22     ; load address of t in X
  MOVW R30, R24     ; load address of r in Z
  MOVW R24, R28     ; save Y register in R24
  MOVW R28, R22     ; put address of upper half of t in Y

  ADIW R26, 15 ; get ptr to byte #15 
  LD R19,X
  SBIW R26, 15 ; restore ptr to byte #0
  ADIW R28, 16 ; make Y point to the upper half
  LDD R22,Y+15

  LSL R22 ; result to accumulate in r0:r1
  ADC R1, R18 ; use the fact that R1 starts empty
  ADD R19, R22
  ADC R1, R18 ; r21 and r1 now hold values for bytes #31 and #32

  LSL R19
  ROL R1 ; R1 now holds the value to be multiplied with 2 
  LSR R19 ; R21 now holds result content for byte #31. Bit #7 cleared.

  LD R21, X+ ; load r0  indirect from RAM (pointer was restored)
  CLR R23
  ADD R21,R1 ; add into lowest byte
  ADC R23,R18
 
  LD R22, Y+
  LSL R22
  ADC R23, R18
  ADD R21, R22
  ADC R23, R18
  ST Z+, R21

  .rept 7  
  LD R21, X+        ; load r1  indirect from RAM
  LD R22, Y+        ; load r17 indirect from RAM
  LSL R22
  ADC R18, R18
  ADD R21, R23
  CLR R23
  ADC R18, R23
  ADD R21, R22
  ADC R18, R23
  ST Z+, R21

  LD R21, X+
  LD R22, Y+
  LSL R22
  ADC R23, R23
  ADD R21, R18
  CLR R18
  ADC R23, R18
  ADD R21, R22
  ADC R23, R18
  ST Z+, R21
  .endr 
  ; OK, byte #14 is finished

  ; finalize byte #15
  ADD R19, R23
  ST Z, R19

  MOVW R28, R24     ; restore Y register

  CLR R1  

  RET
