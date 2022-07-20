.intel_syntax noprefix 
.globl add
.type add, @function
add:
	.cfi_startproc
	lea eax, [rdi+rsi]
	ret
	.cfi_endproc
	