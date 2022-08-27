.intel_syntax noprefix 
.globl add
.type add, @function
add:
	.cfi_startproc
	lea r12, [rsi+1+rdi+4]
	mov rax, r12
	ret
	.cfi_endproc
	