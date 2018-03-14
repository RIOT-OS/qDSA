.global fe1271_sub
.type fe1271_sub, @function

//*********************************************************
// fe1271_sub
// subtracts two 128 bit integers (modulo 2^127 - 1)
//
// Inputs:
//    r      in register R25:R24
//    a      in register R23:R22
//    b      in register R21:R20
//
fe1271_sub:

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
  SUB R7, R8

  LD R8, X+
  LD R9, Y+
  SBC R8, R9

  LD R9, X+
  LD R10, Y+
  SBC R9, R10

  LD R10, X+
  LD R11, Y+
  SBC R10, R11

  LD R11, X+
  LD R12, Y+
  SBC R11, R12

  LD R12, X+
  LD R13, Y+
  SBC R12, R13

  LD R13, X+
  LD R14, Y+
  SBC R13, R14

  LD R14, X+
  LD R15, Y+
  SBC R14, R15

  LD R15, X+
  LD R16, Y+
  SBC R15, R16

  LD R16, X+
  LD R17, Y+
  SBC R16, R17

  LD R17, X+
  LD R18, Y+
  SBC R17, R18

  LD R18, X+
  LD R19, Y+
  SBC R18, R19

  LD R19, X+
  LD R20, Y+
  SBC R19, R20

  LD R20, X+
  LD R21, Y+
  SBC R20, R21

  LD R21, X+
  LD R22, Y+
  SBC R21, R22

  LD R22, X+
  LD R23, Y+
  SBC R22, R23

  CLR R23
  ADC R23, R23 ; store carry
  LSL R23 ; multiply carry by 2
  SUB R7, R23 ; carry through
  SBC R8, R1 ; carry through - R1 always set to 0
  SBC R9, R1 ; carry through 
  SBC R10, R1 ; carry through
  SBC R11, R1 ; carry through
  SBC R12, R1 ; carry through
  SBC R13, R1 ; carry through
  SBC R14, R1 ; carry through
  SBC R15, R1 ; carry through
  SBC R16, R1 ; carry through
  SBC R17, R1 ; carry through
  SBC R18, R1 ; carry through
  SBC R19, R1 ; carry through
  SBC R20, R1 ; carry through
  SBC R21, R1 ; carry through
  SBC R22, R1 ; carry through

  ; can possibly have a final carry bit
  ; in this case the lowest byte must be large 
  CLR R23
  ADC R23, R23
  LSL R23
  SUB R7, R23

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

  MOVW R28, R24             ; restore Y register

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
