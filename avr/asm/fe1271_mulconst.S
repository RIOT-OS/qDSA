; Version: 2015-11-30

.global fe1271_mulconst
.type fe1271_mulconst, @function

; operand b: r21:r20 -- 16 bit positive integer
; operand a: r23:r22
; operand r: r25:r24

fe1271_mulconst:

    PUSH R2
    PUSH R3
    PUSH R6
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
    PUSH R28
    PUSH R29

    MOVW R30, R24
    MOVW R28, R20
    MOVW R26, R22

; init zero registers
    CLR R23

; clear starting registers
    CLR R24 ; instead of R4 to delete PUSH/POP
    CLR R25 ; instead of R5 to delete PUSH/POP
    CLR R6
    CLR R7
    CLR R8
    CLR R9
    CLR R10
    CLR R11
    CLR R12
    CLR R13
    CLR R14
    CLR R15
    CLR R16
    CLR R17
    CLR R18
    CLR R19

; a in X, b in Y
    LDD R21, Y+0
    LDD R22, Y+1

    ; first multiply by low byte

    LD R20, X+ ; R2
    MUL R20, R21
    MOVW R2, R0

    LD R20, X+ ; R3
    MUL R20, R21
    ADD R3, R0
    ADC R24, R1

    LD R20, X+ ; R24
    MUL R20, R21
    ADD R24, R0
    ADC R25, R1

    LD R20, X+ ; R25
    MUL R20, R21
    ADD R25, R0
    ADC R6, R1

    LD R20, X+ ; R6
    MUL R20, R21
    ADD R6, R0
    ADC R7, R1

    LD R20, X+ ; R7
    MUL R20, R21
    ADD R7, R0
    ADC R8, R1

    LD R20, X+ ; R8
    MUL R20, R21
    ADD R8, R0
    ADC R9, R1

    LD R20, X+ ; R9
    MUL R20, R21
    ADD R9, R0
    ADC R10, R1

    LD R20, X+ ; R10
    MUL R20, R21
    ADD R10, R0
    ADC R11, R1

    LD R20, X+ ; R11
    MUL R20, R21
    ADD R11, R0
    ADC R12, R1

    LD R20, X+ ; R12
    MUL R20, R21
    ADD R12, R0
    ADC R13, R1

    LD R20, X+ ; R13
    MUL R20, R21
    ADD R13, R0
    ADC R14, R1

    LD R20, X+ ; R14
    MUL R20, R21
    ADD R14, R0
    ADC R15, R1

    LD R20, X+ ; R15
    MUL R20, R21
    ADD R15, R0
    ADC R16, R1

    LD R20, X+ ; R16
    MUL R20, R21
    ADD R16, R0
    ADC R17, R1

    LD R20, X ; R17 ; points to X+15
    MUL R20, R21
    ADD R17, R0
    ADC R18, R1
    
    ; first do top multiplications, for reduction
    ; reduce X pointer for this to X+13

    SBIW R26, 2 ; point to X+13

    LD R20, X+
    MUL R20, R22
    ADD R16, R0
    ADC R1, R19 ; add carry to R1
    MOV R21, R1 ; save for later

    LD R20, X+
    MUL R20, R22
    ADD R17, R0
    ADC R18, R1
    ADC R19, R19 ; take carry bit to R19

    LD R20, X
    MUL R20, R22
    ADD R17, R21
    ADC R18, R0
    ADC R19, R1 ; cannot overflow

    SBIW R26, 15 ; reset pointer to X+0

    ; multiply the overflows by 2
    ; also take bit 127
    ; top bits of R19 are zero, so do not get into trouble
    LSL R17
    ROL R18
    ROL R19
    LSR R17

    ; high part of MUL will contain many zeros, so no overflow
    LD R20, X+ ; X+0
    MUL R20, R22
    ADD R2, R18
    ADC R3, R0
    MOV R18, R1
    ADC R18, R23 ; store carry
    
    LD R20, X+ ; X+1 
    MUL R20, R22
    ADD R3, R19
    ADC R24, R0
    MOV R19, R1
    ADC R19, R23

    LD R20, X+; X+2 
    MUL R20, R22
    ADD R24, R18
    ADC R25, R0
    MOV R18, R1
    ADC R18, R23

    LD R20, X+; X+3 
    MUL R20, R22
    ADD R25, R19
    ADC R6, R0
    MOV R19, R1
    ADC R19, R23

    LD R20, X+; X+4 
    MUL R20, R22
    ADD R6, R18
    ADC R7, R0
    MOV R18, R1
    ADC R18, R23

    LD R20, X+; X+5 
    MUL R20, R22
    ADD R7, R19
    ADC R8, R0
    MOV R19, R1
    ADC R19, R23

    LD R20, X+; X+6 
    MUL R20, R22
    ADD R8, R18
    ADC R9, R0
    MOV R18, R1
    ADC R18, R23

    LD R20, X+; X+7 
    MUL R20, R22
    ADD R9, R19
    ADC R10, R0
    MOV R19, R1
    ADC R19, R23

    LD R20, X+; X+8
    MUL R20, R22
    ADD R10, R18
    ADC R11, R0
    MOV R18, R1
    ADC R18, R23

    LD R20, X+; X+9 
    MUL R20, R22
    ADD R11, R19
    ADC R12, R0
    MOV R19, R1
    ADC R19, R23

    LD R20, X+; X+10
    MUL R20, R22
    ADD R12, R18
    ADC R13, R0
    MOV R18, R1
    ADC R18, R23

    LD R20, X+; X+11 
    MUL R20, R22
    ADD R13, R19
    ADC R14, R0
    MOV R19, R1
    ADC R19, R23

    ; last round is a bit different
    LD R20, X; X+13 
    MUL R20, R22
    ADD R14, R18
    ADC R15, R0
    ADC R16, R23 ; this can overflow, carry through
    ADC R17, R23 ; cannot overflow
    ADD R15, R19
    ADC R16, R1 ; this can overflow, carry through
    ADC R17, R23 ; cannot overflow

    ; store result
    STD Z+0, R2
    STD Z+1, R3
    STD Z+2, R24
    STD Z+3, R25
    STD Z+4, R6
    STD Z+5, R7
    STD Z+6, R8
    STD Z+7, R9
    STD Z+8, R10
    STD Z+9, R11
    STD Z+10, R12
    STD Z+11, R13
    STD Z+12, R14
    STD Z+13, R15
    STD Z+14, R16
    STD Z+15, R17

    CLR R1 
    POP R29
    POP R28
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
    POP R6
    POP R3
    POP R2

RET
