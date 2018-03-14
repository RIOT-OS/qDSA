 .align	2
	.global fe1271_mulconst	
	.type	fe1271_mulconst, %function

@ R0 = r
@ R1 = x (address)
@ R2 = y (value, < 16 bits)
fe1271_mulconst:
	push {r4-r7,lr}

    LDRH R3, [R1]
    MUL R3, R2

    LDRH R4, [R1, #2]
    MUL R4, R2
    LSL R6, R4, #16
    LSR R7, R4, #16
    ADD R3, R6 

    LDRH R4, [R1, #4]
    MUL R4, R2
    ADC R4, R7

    LDRH R5, [R1, #6]
    MUL R5, R2
    LSL R6, R5, #16
    LSR R7, R5, #16
    ADD R4, R6

    LDRH R5, [R1, #8]
    MUL R5, R2
    ADC R5, R7

    LDRH R6, [R1, #10]
    MUL R6, R2
    LSR R7, R6, #16
    LSL R6, #16
    ADD R5, R6

    LDRH R6, [R1, #12]
    MUL R6, R2
    ADC R6, R7

    LDRH R7, [R1, #14]
    MUL R7, R2
    LSL R1, R7, #16
    LSR R7, #16
    ADD R6, R1

    BIC R2, R2
    ADC R7, R2
    LSL R7, #1

    LSL R6, #1
    ADC R7, R2
    LSR R6, #1

    ADD R3, R7
    ADC R4, R2
    ADC R5, R2
    ADC R6, R2

    STM R0!, {R3,R4,R5,R6}

	pop {r4-r7,pc}
	bx	lr
.size	fe1271_mulconst, .-fe1271_mulconst
