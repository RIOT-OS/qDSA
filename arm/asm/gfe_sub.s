 .align	2
	.global gfe_sub
	.type	gfe_sub, %function

@ R0 = r
@ R1 = x
@ R2 = y
gfe_sub:
	push {r4-r7,lr}

    LDM R1!, {R3, R4, R5, R6}
    LDM R2!, {R1, R7}

    SUB R3, R1
    SBC R4, R7

    LDM R2!, {R1, R7}

    SBC R5, R1
    SBC R6, R7

    BIC R2, R2
    SBC R2, R2 @ take carry
    .syntax unified
    RSBS R2, #0
    .syntax divided
    LSL R2, #1

    BIC R7, R7
    SUB R3, R2
    SBC R4, R7
    SBC R5, R7
    SBC R6, R7

    SBC R7, R7 @ take carry
    .syntax unified
    RSBS R7, #0
    .syntax divided
    LSL R7, #1
    SUB R3, R7

    STM R0!, {R3, R4, R5, R6}

	pop {r4-r7,pc}
	bx	lr
.size	gfe_sub, .-gfe_sub
