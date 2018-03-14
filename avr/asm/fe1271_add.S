.global fe1271_add
.type fe1271_add, @function

//*********************************************************
// fe1271_add
// addtracts two 128 bit integers (modulo 2^127-1)
//
// Inputs:
//    r      in register R25:R24
//    a      in register R23:R22
//    b      in register R21:R20
//

fe1271_add:

  PUSH R7
  PUSH R8
  PUSH R9
  PUSH R10
  PUSH R11
  PUSH R12
  PUSH R13
  PUSH R14
  PUSH R15
  PUSH R16
  PUSH R17

  MOVW R26, R22             ; load operand address a to X
  MOVW R30, R24             ; load address of result to Z
  MOVW R24, R28             ; save Y register in R24
  MOVW R28, R20             ; load operand address b to Y
  
  LD R7, X+
  LD R8, Y+
  ADD R7, R8

  LD R8, X+
  LD R9, Y+
  ADC R8, R9

  LD R9, X+
  LD R10, Y+
  ADC R9, R10

  LD R10, X+
  LD R11, Y+
  ADC R10, R11

  LD R11, X+
  LD R12, Y+
  ADC R11, R12

  LD R12, X+
  LD R13, Y+
  ADC R12, R13

  LD R13, X+
  LD R14, Y+
  ADC R13, R14

  LD R14, X+
  LD R15, Y+
  ADC R14, R15

  LD R15, X+
  LD R16, Y+
  ADC R15, R16

  LD R16, X+
  LD R17, Y+
  ADC R16, R17

  LD R17, X+
  LD R18, Y+
  ADC R17, R18

  LD R18, X+
  LD R19, Y+
  ADC R18, R19

  LD R19, X+
  LD R20, Y+
  ADC R19, R20

  LD R20, X+
  LD R21, Y+
  ADC R20, R21

  LD R21, X+
  LD R22, Y+
  ADC R21, R22

  LD R22, X+
  LD R23, Y+
  ADC R22, R23

  CLR R23
  ADC R23, R23 ; store carry
  LSL R22
  ROL R23
  LSR R22

  ADD R7, R23 ; carry through
  ADC R8, R1 ; carry through - R1 always set to 0
  ADC R9, R1 ; carry through
  ADC R10, R1 ; carry through
  ADC R11, R1 ; carry through
  ADC R12, R1 ; carry through
  ADC R13, R1 ; carry through
  ADC R14, R1 ; carry through
  ADC R15, R1 ; carry through
  ADC R16, R1 ; carry through
  ADC R17, R1 ; carry through
  ADC R18, R1 ; carry through
  ADC R19, R1 ; carry through
  ADC R20, R1 ; carry through
  ADC R21, R1 ; carry through
  ADC R22, R1 ; carry through

  ST Z+, R7
  ST Z+, R8
  ST Z+, R9
  ST Z+, R10
  ST Z+, R11
  ST Z+, R12
  ST Z+, R13
  ST Z+, R14
  ST Z+, R15
  ST Z+, R16
  ST Z+, R17
  ST Z+, R18
  ST Z+, R19
  ST Z+, R20
  ST Z+, R21
  ST Z, R22

  MOVW R28, R24 ; restore Y register

  CLR R1    
  POP R17
  POP R16
  POP R15
  POP R14
  POP R13
  POP R12
  POP R11
  POP R10
  POP R9
  POP R8
  POP R7

  RET
