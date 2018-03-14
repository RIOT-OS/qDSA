.global fe1271_neg
.type fe1271_neg, @function

//*********************************************************
// fe1271_neg
// negates a 128 bit integer (modulo 2^127 - 1)
//
// Inputs:
//    r      in register R25:R24
//    a      in register R23:R22
//

fe1271_neg:

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

  ; clear register for subtraction
  
  CLR R10
  CLR R11
  MOVW R12, R10
  MOVW R14, R10
  MOVW R16, R10
  MOVW R18, R10
  MOVW R20, R10
  MOVW R22, R10
  MOVW R24, R10

  LD R9, X+ ; byte 0
  SUB R10, R9

  LD R9, X+ ; byte 1
  SBC R11, R9

  LD R9, X+ ; byte 2
  SBC R12, R9

  LD R9, X+ ; byte 3
  SBC R13, R9

  LD R9, X+ ; byte 4
  SBC R14, R9

  LD R9, X+ ; byte 5
  SBC R15, R9

  LD R9, X+ ; byte 6
  SBC R16, R9

  LD R9, X+ ; byte 7
  SBC R17, R9

  LD R9, X+ ; byte 8
  SBC R18, R9
  
  LD R9, X+ ; byte 9
  SBC R19, R9
  
  LD R9, X+ ; byte 10
  SBC R20, R9
  
  LD R9, X+ ; byte 11
  SBC R21, R9
  
  LD R9, X+ ; byte 12
  SBC R22, R9
  
  LD R9, X+ ; byte 13
  SBC R23, R9
  
  LD R9, X+ ; byte 14
  SBC R24, R9
  
  LD R9, X+ ; byte 15
  SBC R25, R9

  ADC R1, R1 ; store carry
  LSL R1 ; multiply carry by 2
  SUB R10, R1 ; carry through
  CLR R1
  SBC R11, R1 ; carry through - R1 always set to 0
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
  SBC R23, R1 ; carry through
  SBC R24, R1 ; carry through
  SBC R25, R1 ; carry through

  ADC R1, R1
  LSL R1
  SUB R10, R1

  CLR R1

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
  ST Z+, R22
  ST Z+, R23
  ST Z+, R24
  ST Z, R25

  POP R17
  POP R16
  POP R15
  POP R14
  POP R13
  POP R12
  POP R11
  POP R10
  POP R9

  RET
