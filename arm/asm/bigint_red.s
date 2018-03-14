 .align	2
	.global bigint_red
	.type	bigint_red, %function

@ R0 = r
@ R1 = x
bigint_red:
	push {r4-r7,lr}

    BIC R7, R7 @ use as empty reg

    LDM R1!, {R4, R5, R6, R7} @ lower half
    MOV R8, R4
    MOV R9, R5
    LDM R1!, {R2, R3, R4, R5} @ upper half
    
    @ first double upper half, save carry into R1
    BIC R1, R1 @ for carries
    ADD R2, R2
    ADC R3, R3
    ADC R4, R4
    ADC R5, R5
    ADC R1, R1

    @ do top word first, to avoid carry later
    ADD R7, R5
    BIC R5, R5
    ADC R1, R5 @ store carry in R1
    LSL R1, #1 @ multiply by 2
    LSL R7, #1
    ADC R1, R5 @ top bit into R1
    LSR R7, #1

    @ now start from bottom again
    MOV R5, R8
    ADD R5, R2
    MOV R2, R9
    ADC R2, R3
    ADC R6, R4
    BIC R3, R3
    ADC R7, R3

    @STM R0!, {R5, R2, R6, R7}

    @ finally do carry bits
    ADD R5, R1
    ADC R2, R3
    ADC R6, R3
    ADC R7, R3 @ cannot overflow

    MOV R1, R5
    STM R0!, {R1, R2, R6, R7}

	pop {r4-r7,pc}
	bx	lr
.size	bigint_red, .-bigint_red
