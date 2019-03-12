	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14
	.globl	_hostname_to_ip         ## -- Begin function hostname_to_ip
	.p2align	4, 0x90
_hostname_to_ip:                        ## @hostname_to_ip
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	pushq	%rax
	.cfi_offset %rbx, -24
	movq	%rsi, %rbx
	callq	_gethostbyname
	movq	%rax, %rcx
	movl	$1, %eax
	testq	%rcx, %rcx
	je	LBB0_3
## %bb.1:
	movq	24(%rcx), %rcx
	movq	(%rcx), %rcx
	testq	%rcx, %rcx
	je	LBB0_3
## %bb.2:
	movl	(%rcx), %edi
	callq	_inet_ntoa
	movq	%rbx, %rdi
	movq	%rax, %rsi
	callq	_strcpy
	xorl	%eax, %eax
LBB0_3:
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
