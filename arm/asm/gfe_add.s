 .align	2
	.global gfe_add 
	.type	gfe_add, %function

@ R0 = r
@ R1 = x
@ R2 = y
gfe_add:
	push {r4-r7,lr}

    LDM R1!, {R3, R4, R5, R6}
    LDM R2!, {R1, R7}

    ADD R3, R1
    ADC R4, R7

    LDM R2!, {R1, R7}

    ADC R5, R1
    ADC R6, R7

    BIC R2, R2
    ADC R2, R2 @ take carry
    LSL R2, #1
    LSR R7, R6, #31 @ top bit
    LSL R6, #1
    LSR R6, #1
    EOR R2, R7

    BIC R7, R7
    ADD R3, R2
    ADC R4, R7
    ADC R5, R7
    ADC R6, R7

    STM R0!, {R3, R4, R5, R6}

	pop {r4-r7,pc}
	bx	lr
.size	gfe_add, .-gfe_add
