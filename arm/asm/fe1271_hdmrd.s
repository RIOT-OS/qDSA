 .align	2
	.global fe1271_hdmrd
	.type	fe1271_hdmrd, %function

@ R0 = r
@ R1 = x
fe1271_hdmrd:
	push {r4-r7,lr}

    LDM R1!, {R2, R3, R4, R5}
    MOV R8, R2
    MOV R9, R3
    MOV R10, R4
    MOV R11, R5

    LDM R1!, {R2, R3, R4, R5}
    MOV R6, R8
    MOV R7, R9
    ADD R2, R6
    ADC R3, R7
    MOV R6, R10
    MOV R7, R11
    ADC R4, R6
    ADC R5, R7
    BIC R6, R6
    ADC R6, R6
    LSL R6, #1
    LSR R7, R5, #31
    LSL R5, #1
    LSR R5, #1
    EOR R6, R7
    BIC R7, R7
    ADD R2, R6
    ADC R3, R7
    ADC R4, R7
    ADC R5, R7
    STM R0!, {R2, R3, R4, R5} @ a[0]+a[1]

    SUB R1, #16 @ restore pointer
    LDM R1!, {R2, R3, R4, R5};
    MOV R6, R8
    MOV R7, R9
    SUB R2, R6
    SBC R3, R7
    MOV R6, R10
    MOV R7, R11
    SBC R4, R6
    SBC R5, R7
    BIC R6, R6
    SBC R6, R6 @ take carry
    .syntax unified
    RSBS R6, #0
    .syntax divided
    LSL R6, #1
    BIC R7, R7
    SUB R2, R6
    SBC R3, R7
    SBC R4, R7
    SBC R5, R7
    SBC R7, R7 @ take carry
    .syntax unified
    RSBS R7, #0
    .syntax divided
    LSL R7, #1
    SUB R3, R7
    STM R0!, {R2, R3, R4, R5} @ a[1]-a[0]

    LDM R1!, {R2, R3, R4, R5} @ a[2]
    LDM R1!, {R6, R7} @ a[3] lower two words
    ADD R2, R6
    ADC R3, R7
    LDM R1!, {R6, R7} @ a[3] upper two words
    ADC R4, R6
    ADC R5, R7
    BIC R6, R6
    ADC R6, R6
    LSL R6, #1
    LSR R7, R5, #31
    LSL R5, #1
    LSR R5, #1
    EOR R6, R7
    BIC R7, R7
    ADD R2, R6
    ADC R3, R7
    ADC R4, R7
    ADC R5, R7
    MOV R8, R2 @ store for later
    MOV R9, R3
    MOV R10, R4
    MOV R11, R5

    SUB R1, #32 @ restore pointer
    LDM R1!, {R2, R3, R4, R5} @ a[2]
    LDM R1!, {R6, R7} @ a[3] lower two words
    SUB R2, R6
    SBC R3, R7
    LDM R1!, {R6, R7} @ a[3] upper two words
    SBC R4, R6
    SBC R5, R7
    BIC R6, R6
    SBC R6, R6 @ take carry
    .syntax unified
    RSBS R6, #0
    .syntax divided
    LSL R6, #1
    BIC R7, R7
    SUB R2, R6
    SBC R3, R7
    SBC R4, R7
    SBC R5, R7
    SBC R7, R7 @ take carry
    .syntax unified
    RSBS R7, #0
    .syntax divided
    LSL R7, #1
    SUB R3, R7
    ADD R0, #16
    STM R0!, {R2, R3, R4, R5}

    @ c = a[1]+a[0] in r[0]
    @ a = a[1]-a[0] in r[1]
    @ b = a[2]+a[3] in [R8,R9,R10,R11]
    @ d = a[2]-a[3] in [R2,R3,R4,R5] and r[3]

    SUB R0, #64 @ restore pointer to c in r[0]
    LDM R0!, {R6, R7}
    SUB R2, R6
    SBC R3, R7
    LDM R0!, {R6, R7}
    SBC R4, R6
    SBC R5, R7
    BIC R6, R6
    SBC R6, R6 @ take carry
    .syntax unified
    RSBS R6, #0
    .syntax divided
    LSL R6, #1
    BIC R7, R7
    SUB R2, R6
    SBC R3, R7
    SBC R4, R7
    SBC R5, R7
    SBC R7, R7 @ take carry
    .syntax unified
    RSBS R7, #0
    .syntax divided
    LSL R7, #1
    SUB R3, R7
    ADD R0, #16 @ pointer to r[2]
    STM R0!, {R2, R3, R4, R5}

    LDM R0!, {R2, R3, R4, R5} @ load d
    SUB R0, #64 @ restore pointer to c in r[0]
    LDM R0!, {R6, R7}
    ADD R2, R6
    ADC R3, R7
    LDM R0!, {R6, R7}
    ADC R4, R6
    ADC R5, R7
    BIC R6, R6
    ADC R6, R6
    LSL R6, #1
    LSR R7, R5, #31
    LSL R5, #1
    LSR R5, #1
    EOR R6, R7
    BIC R7, R7
    ADD R2, R6
    ADC R3, R7
    ADC R4, R7
    ADC R5, R7
    ADD R0, #32 @ pointer to r[3]
    STM R0!, {R2, R3, R4, R5}

    SUB R0, #48 @ pointer to r[1]
    LDM R0!, {R2, R3, R4, R5}
    MOV R6, R8
    MOV R7, R9
    ADD R2, R6
    ADC R3, R7
    MOV R6, R10
    MOV R7, R11
    ADC R4, R6
    ADC R5, R7
    BIC R6, R6
    ADC R6, R6
    LSL R6, #1
    LSR R7, R5, #31
    LSL R5, #1
    LSR R5, #1
    EOR R6, R7
    BIC R7, R7
    ADD R2, R6
    ADC R3, R7
    ADC R4, R7
    ADC R5, R7
    SUB R0, #32 @ pointer to r[0]
    STM R0!, {R2, R3, R4, R5}

    LDM R0!, {R2, R3, R4, R5}
    MOV R6, R8
    MOV R7, R9
    SUB R2, R6
    SBC R3, R7
    MOV R6, R10
    MOV R7, R11
    SBC R4, R6
    SBC R5, R7
    BIC R6, R6
    SBC R6, R6 @ take carry
    .syntax unified
    RSBS R6, #0
    .syntax divided
    LSL R6, #1
    BIC R7, R7
    SUB R2, R6
    SBC R3, R7
    SBC R4, R7
    SBC R5, R7
    SBC R7, R7 @ take carry
    .syntax unified
    RSBS R7, #0
    .syntax divided
    LSL R7, #1
    SUB R3, R7
    SUB R0, #16
    STM R0!, {R2, R3, R4, R5}

	pop {r4-r7,pc}
	bx	lr
.size	fe1271_hdmrd, .-fe1271_hdmrd
