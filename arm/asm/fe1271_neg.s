 .align	2
	.global fe1271_neg
	.type	fe1271_neg, %function

@ R0 = r
@ R1 = x
fe1271_neg:
	push {r4-r7,lr}

    LDM R1!, {R3, R4, R5, R6}

    .syntax unified
    RSBS R1, R3, #0
    .syntax divided
    BIC R2, R2
    SBC R2, R4
    BIC R3, R3
    SBC R3, R5
    BIC R7, R7
    SBC R7, R6

    BIC R4, R4
    SBC R4, R4
    .syntax unified
    RSBS R4, #0
    .syntax divided
    LSL R4, #1
    SUB R1, R4

    BIC R4, R4
    SBC R2, R4
    SBC R3, R4
    SBC R7, R4

    STM R0!, {R1, R2, R3, R7}

	pop {r4-r7,pc}
	bx	lr
.size	fe1271_neg, .-fe1271_neg
