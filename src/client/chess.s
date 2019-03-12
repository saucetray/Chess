	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14
	.globl	_create_newwin          ## -- Begin function create_newwin
	.p2align	4, 0x90
_create_newwin:                         ## @create_newwin
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
	callq	_newwin
	movq	%rax, %rbx
	movq	%rbx, %rdi
	callq	_wrefresh
	movq	%rbx, %rax
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4               ## -- Begin function connect_to_server
LCPI1_0:
	.space	16,48
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_connect_to_server
	.p2align	4, 0x90
_connect_to_server:                     ## @connect_to_server
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$88, %rsp
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	movl	%edi, %eax
	shrl	$31, %eax
	addl	%edi, %eax
	sarl	%eax
	movq	%rax, -120(%rbp)        ## 8-byte Spill
	leal	-3(%rax), %eax
	movl	%eax, -88(%rbp)         ## 4-byte Spill
	movslq	%esi, %rax
	movabsq	$8589934575, %rcx       ## imm = 0x1FFFFFFEF
	addq	%rax, %rcx
	shrq	%rcx
	movq	%rcx, -112(%rbp)        ## 8-byte Spill
	movq	%rax, -96(%rbp)         ## 8-byte Spill
	leaq	-25(%rax), %rax
	movabsq	$-6148914691236517205, %rcx ## imm = 0xAAAAAAAAAAAAAAAB
	mulq	%rcx
	shrq	%rdx
	movq	%rdx, -104(%rbp)        ## 8-byte Spill
	movq	_stdscr@GOTPCREL(%rip), %r15
	xorl	%r14d, %r14d
                                        ## implicit-def: %eax
                                        ## kill: killed %eax
	.p2align	4, 0x90
LBB1_1:                                 ## =>This Inner Loop Header: Depth=1
	movq	(%r15), %rdi
	movl	$2097152, %esi          ## imm = 0x200000
	xorl	%edx, %edx
	callq	_wattr_on
	leal	-1(%r14), %eax
	cmpl	$1, %eax
	leaq	L_.str.1(%rip), %r12
	ja	LBB1_3
## %bb.2:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$1, %edi
	movl	$1, %esi
	xorl	%edx, %edx
	callq	_init_pair
	movq	(%r15), %rdi
	movl	$256, %esi              ## imm = 0x100
	xorl	%edx, %edx
	callq	_wattr_on
	cmpl	$1, %r14d
	leaq	L_.str.18(%rip), %rcx
	leaq	L_.str.17(%rip), %rax
	cmoveq	%rax, %rcx
	movl	$60, %eax
	movl	$70, %edx
	cmoveq	%rdx, %rax
	movq	-96(%rbp), %rsi         ## 8-byte Reload
	subq	%rax, %rsi
	shrq	%rsi
	xorl	%eax, %eax
	movl	-88(%rbp), %edi         ## 4-byte Reload
                                        ## kill: def %esi killed %esi killed %rsi
	movq	%r12, %rdx
	callq	_mvprintw
	movq	(%r15), %rdi
	callq	_wrefresh
	movq	(%r15), %rdi
	movl	$256, %esi              ## imm = 0x100
	xorl	%edx, %edx
	callq	_wattr_off
LBB1_3:                                 ##   in Loop: Header=BB1_1 Depth=1
	movl	$1, %edi
	xorl	%eax, %eax
	movq	-112(%rbp), %rsi        ## 8-byte Reload
                                        ## kill: def %esi killed %esi killed %rsi
	movq	%r12, %rdx
	leaq	L_.str.16(%rip), %rcx
	callq	_mvprintw
	movq	(%r15), %rdi
	xorl	%edx, %edx
	movq	-120(%rbp), %rbx        ## 8-byte Reload
	movl	%ebx, %esi
	callq	_wmove
	movq	(%r15), %rdi
	callq	_wclrtoeol
	xorl	%eax, %eax
	movl	%ebx, %edi
	movq	-104(%rbp), %rsi        ## 8-byte Reload
                                        ## kill: def %esi killed %esi killed %rsi
	movq	%r12, %rdx
	leaq	L_.str.15(%rip), %rcx
	callq	_mvprintw
	movl	$1, %edi
	movl	$60, %esi
	callq	_calloc
	movq	%rax, %rbx
	movq	(%r15), %rdi
	callq	_wrefresh
	movq	(%r15), %rdi
	movl	$-1, %r13d
	movl	$-1, %edx
	movq	%rbx, %rsi
	callq	_wgetnstr
	cmpb	$0, (%rbx)
	je	LBB1_14
## %bb.4:                               ##   in Loop: Header=BB1_1 Depth=1
	movq	%rbx, %rdi
	leaq	L_.str(%rip), %r15
	movq	%r15, %rsi
	callq	_strtok
	movq	%rax, -128(%rbp)        ## 8-byte Spill
	xorl	%edi, %edi
	movq	%r15, %rsi
	callq	_strtok
	movq	%rax, %r12
	movq	%rbx, %rdi
	leaq	-64(%rbp), %rsi
	callq	_hostname_to_ip
	cmpl	$1, %eax
	jne	LBB1_6
## %bb.5:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$1, %r14d
	movl	-84(%rbp), %r13d        ## 4-byte Reload
	testl	%r14d, %r14d
	movq	_stdscr@GOTPCREL(%rip), %r15
	jne	LBB1_1
	jmp	LBB1_14
	.p2align	4, 0x90
LBB1_6:                                 ##   in Loop: Header=BB1_1 Depth=1
	testq	%r12, %r12
	je	LBB1_13
## %bb.7:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$2, %r14d
	movl	$2, %edi
	movl	$4, %esi
	xorl	%edx, %edx
	callq	_init_pair
	movq	_stdscr@GOTPCREL(%rip), %rax
	movq	(%rax), %rdi
	movl	$512, %esi              ## imm = 0x200
	xorl	%edx, %edx
	callq	_wattr_on
	xorl	%edi, %edi
	xorl	%esi, %esi
	xorl	%eax, %eax
	leaq	L_.str.1(%rip), %r15
	movq	%r15, %rdx
	leaq	L_.str.2(%rip), %rcx
	callq	_mvprintw
	movl	$1, %edi
	xorl	%esi, %esi
	xorl	%eax, %eax
	movq	%r15, %rdx
	leaq	L_.str.3(%rip), %rcx
	callq	_mvprintw
	movl	$1, %edi
	xorl	%esi, %esi
	xorl	%eax, %eax
	movq	%r15, %rdx
	leaq	L_.str.4(%rip), %rcx
	callq	_mvprintw
	leaq	-64(%rbp), %rdi
	movq	%rbx, %rsi
	callq	_strcmp
	testl	%eax, %eax
	je	LBB1_9
## %bb.8:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$2, %edi
	xorl	%esi, %esi
	xorl	%eax, %eax
	movq	%r15, %rdx
	leaq	L_.str.5(%rip), %rcx
	callq	_mvprintw
	xorl	%eax, %eax
	movq	%r15, %rdi
	movq	-128(%rbp), %rsi        ## 8-byte Reload
	callq	_printw
	movl	$3, %r14d
LBB1_9:                                 ##   in Loop: Header=BB1_1 Depth=1
	xorl	%esi, %esi
	xorl	%eax, %eax
	movl	%r14d, %edi
	movq	%r15, %rdx
	leaq	L_.str.6(%rip), %rcx
	callq	_mvprintw
	xorl	%eax, %eax
	movq	%r15, %rdi
	leaq	-64(%rbp), %rsi
	callq	_printw
	incl	%r14d
	xorl	%esi, %esi
	xorl	%eax, %eax
	movl	%r14d, %edi
	movq	%r15, %rdx
	leaq	L_.str.7(%rip), %rcx
	callq	_mvprintw
	xorl	%eax, %eax
	movq	%r15, %rdi
	movq	%r12, %rsi
	callq	_printw
	movq	_stdscr@GOTPCREL(%rip), %rbx
	movq	(%rbx), %rdi
	callq	_wrefresh
	movq	(%rbx), %rdi
	movl	$512, %esi              ## imm = 0x200
	xorl	%edx, %edx
	callq	_wattr_off
	movaps	LCPI1_0(%rip), %xmm0    ## xmm0 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	movaps	%xmm0, -80(%rbp)
	movl	$2, %edi
	movl	$1, %esi
	xorl	%edx, %edx
	callq	_socket
	movl	%eax, -84(%rbp)         ## 4-byte Spill
	testl	%eax, %eax
	js	LBB1_14
## %bb.10:                              ##   in Loop: Header=BB1_1 Depth=1
	movb	$2, -79(%rbp)
	xorl	%esi, %esi
	xorl	%edx, %edx
	movq	%r12, %rdi
	callq	_strtol
	rolw	$8, %ax
	movw	%ax, -78(%rbp)
	movl	$2, %edi
	leaq	-64(%rbp), %rsi
	leaq	-76(%rbp), %rdx
	callq	_inet_pton
	movl	$2, %edi
	callq	_sleep
	movl	$16, %edx
	movl	-84(%rbp), %edi         ## 4-byte Reload
	leaq	-80(%rbp), %rsi
	callq	_connect
	movq	(%rbx), %rdi
	testl	%eax, %eax
	js	LBB1_11
## %bb.12:                              ##   in Loop: Header=BB1_1 Depth=1
	xorl	%r14d, %r14d
	movl	$512, %esi              ## imm = 0x200
	xorl	%edx, %edx
	callq	_wattr_on
	movl	$1, %edi
	xorl	%esi, %esi
	xorl	%eax, %eax
	leaq	L_.str.1(%rip), %rdx
	leaq	L_.str.9(%rip), %rcx
	callq	_mvprintw
	movq	(%rbx), %rdi
	movl	$512, %esi              ## imm = 0x200
	xorl	%edx, %edx
	callq	_wattr_off
	.p2align	4, 0x90
LBB1_13:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-84(%rbp), %r13d        ## 4-byte Reload
	testl	%r14d, %r14d
	movq	_stdscr@GOTPCREL(%rip), %r15
	jne	LBB1_1
	jmp	LBB1_14
LBB1_11:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	$256, %esi              ## imm = 0x100
	xorl	%edx, %edx
	callq	_wattr_on
	movl	$1, %edi
	xorl	%esi, %esi
	xorl	%eax, %eax
	leaq	L_.str.1(%rip), %rdx
	leaq	L_.str.8(%rip), %rcx
	callq	_mvprintw
	movq	(%rbx), %rdi
	movl	$256, %esi              ## imm = 0x100
	xorl	%edx, %edx
	callq	_wattr_off
	movl	$2, %r14d
	movl	-84(%rbp), %r13d        ## 4-byte Reload
	testl	%r14d, %r14d
	movq	_stdscr@GOTPCREL(%rip), %r15
	jne	LBB1_1
LBB1_14:
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	cmpq	-48(%rbp), %rax
	jne	LBB1_16
## %bb.15:
	movl	%r13d, %eax
	addq	$88, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
LBB1_16:
	callq	___stack_chk_fail
	.cfi_endproc
                                        ## -- End function
	.globl	_send_credentials       ## -- Begin function send_credentials
	.p2align	4, 0x90
_send_credentials:                      ## @send_credentials
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r12
	pushq	%rbx
	subq	$16, %rsp
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	%rdx, %r14
	movq	%rsi, %r15
	movl	%edi, %ebx
	movq	_stdscr@GOTPCREL(%rip), %r12
	movq	(%r12), %rdi
	callq	_wclear
	movq	(%r12), %rdi
	callq	_wrefresh
	movl	$42, %edx
	movl	%ebx, %edi
	movq	%r15, %rsi
	callq	_write
	leaq	-36(%rbp), %rsi
	movl	$4, %edx
	movl	%ebx, %edi
	callq	_read
	movl	$-1, %eax
	cmpl	$-5, -36(%rbp)
	je	LBB2_2
## %bb.1:
	movl	$42, %edx
	movl	%ebx, %edi
	movq	%r14, %rsi
	callq	_write
	leaq	-36(%rbp), %rsi
	movl	$4, %edx
	movl	%ebx, %edi
	callq	_read
	xorl	%eax, %eax
	cmpl	$-5, -36(%rbp)
	setne	%al
	leal	-2(%rax,%rax,2), %eax
LBB2_2:
	addq	$16, %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_login_server           ## -- Begin function login_server
	.p2align	4, 0x90
_login_server:                          ## @login_server
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$168, %rsp
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
                                        ## kill: def %esi killed %esi def %rsi
	movl	%edi, -148(%rbp)        ## 4-byte Spill
	movl	%esi, %edi
	shrl	$31, %edi
	addl	%esi, %edi
	sarl	%edi
	leal	3(%rdi), %eax
	movl	%eax, -164(%rbp)        ## 4-byte Spill
	movslq	%edx, %rax
	movabsq	$8589934582, %rdx       ## imm = 0x1FFFFFFF6
	leaq	5(%rax,%rdx), %rcx
	shrq	%rcx
	movq	%rcx, -208(%rbp)        ## 8-byte Spill
	leal	1(%rdi), %ecx
	movl	%ecx, -160(%rbp)        ## 4-byte Spill
	addq	%rax, %rdx
	shrq	%rdx
	movq	%rdx, -192(%rbp)        ## 8-byte Spill
	decl	%edi
	movq	%rdi, -200(%rbp)        ## 8-byte Spill
	leal	1(%rsi), %ecx
	movl	%ecx, -156(%rbp)        ## 4-byte Spill
	leal	11(%rax), %ecx
	movl	%ecx, -152(%rbp)        ## 4-byte Spill
	decl	%esi
	movq	%rsi, -184(%rbp)        ## 8-byte Spill
	addq	$10, %rax
	shrq	%rax
	incl	%eax
	movq	%rax, -176(%rbp)        ## 8-byte Spill
	movq	_stdscr@GOTPCREL(%rip), %r15
	leaq	-96(%rbp), %r12
	leaq	-144(%rbp), %r14
	xorl	%r13d, %r13d
	.p2align	4, 0x90
LBB3_1:                                 ## =>This Inner Loop Header: Depth=1
	xorps	%xmm0, %xmm0
	movaps	%xmm0, -80(%rbp)
	movaps	%xmm0, -96(%rbp)
	movw	$0, -56(%rbp)
	movq	$0, -64(%rbp)
	movaps	%xmm0, -128(%rbp)
	movaps	%xmm0, -144(%rbp)
	movw	$0, -104(%rbp)
	movq	$0, -112(%rbp)
	xorl	%eax, %eax
	movl	-164(%rbp), %edi        ## 4-byte Reload
	movq	-208(%rbp), %rsi        ## 8-byte Reload
                                        ## kill: def %esi killed %esi killed %rsi
	leaq	L_.str.1(%rip), %rbx
	movq	%rbx, %rdx
	leaq	L_.str.10(%rip), %rcx
	callq	_mvprintw
	xorl	%eax, %eax
	movl	-160(%rbp), %edi        ## 4-byte Reload
	movq	%r15, %rbx
	movq	-192(%rbp), %r15        ## 8-byte Reload
	movl	%r15d, %esi
	leaq	L_.str.1(%rip), %rdx
	leaq	L_.str.11(%rip), %rcx
	callq	_mvprintw
	xorl	%eax, %eax
	movq	-200(%rbp), %rdi        ## 8-byte Reload
                                        ## kill: def %edi killed %edi killed %rdi
	movl	%r15d, %esi
	movq	%rbx, %r15
	leaq	L_.str.1(%rip), %rdx
	leaq	L_.str.12(%rip), %rcx
	callq	_mvprintw
	movq	(%r15), %rdi
	movl	-156(%rbp), %esi        ## 4-byte Reload
	movl	-152(%rbp), %edx        ## 4-byte Reload
	callq	_wmove
	movq	(%r15), %rdi
	movl	$-1, %edx
	movq	%r12, %rbx
	movq	%r12, %rsi
	callq	_wgetnstr
	cmpb	$0, -96(%rbp)
	je	LBB3_3
## %bb.2:                               ##   in Loop: Header=BB3_1 Depth=1
	movq	(%r15), %rdi
	movq	-184(%rbp), %rsi        ## 8-byte Reload
                                        ## kill: def %esi killed %esi killed %rsi
	movq	-176(%rbp), %rdx        ## 8-byte Reload
                                        ## kill: def %edx killed %edx killed %rdx
	callq	_wmove
	movq	(%r15), %rdi
	movl	$-1, %edx
	movq	%r14, %rsi
	callq	_wgetnstr
	cmpb	$0, -144(%rbp)
	je	LBB3_3
## %bb.4:                               ##   in Loop: Header=BB3_1 Depth=1
	movl	-148(%rbp), %edi        ## 4-byte Reload
	movq	%rbx, %r12
	movq	%r12, %rsi
	movq	%r14, %rdx
	callq	_send_credentials
	testl	%eax, %eax
	movl	$1, %eax
	cmovsl	%eax, %r13d
	testl	%r13d, %r13d
	jne	LBB3_1
	jmp	LBB3_5
LBB3_3:
	movl	$-1, %eax
LBB3_5:
	movq	___stack_chk_guard@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rcx
	cmpq	-48(%rbp), %rcx
	jne	LBB3_7
## %bb.6:
	addq	$168, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
LBB3_7:
	callq	___stack_chk_fail
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r12
	pushq	%rbx
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	callq	_initscr
	callq	_raw
	movq	_stdscr@GOTPCREL(%rip), %r12
	movq	(%r12), %rdi
	movl	$1, %r14d
	movl	$1, %esi
	callq	_keypad
	movq	(%r12), %rdi
	callq	_getmaxy
	movl	%eax, %ebx
	movq	(%r12), %rdi
	callq	_getmaxx
	movl	%eax, %r15d
	cmpl	$40, %ebx
	jl	LBB4_2
## %bb.1:
	cmpl	$39, %r15d
	jle	LBB4_2
## %bb.3:
	callq	_has_colors
	testb	%al, %al
	je	LBB4_4
## %bb.5:
	callq	_start_color
	movl	%ebx, %edi
	movl	%r15d, %esi
	callq	_connect_to_server
	movl	%eax, %edi
	movl	%ebx, %esi
	movl	%r15d, %edx
	callq	_login_server
	movq	(%r12), %rdi
	callq	_wrefresh
	callq	_endwin
	xorl	%r14d, %r14d
	jmp	LBB4_6
LBB4_2:
	movq	(%r12), %rdi
	movl	$2097152, %esi          ## imm = 0x200000
	xorl	%edx, %edx
	callq	_wattr_on
	movl	%ebx, %edi
	shrl	$31, %edi
	addl	%ebx, %edi
	sarl	%edi
	movslq	%r15d, %rax
	movabsq	$8589934535, %rsi       ## imm = 0x1FFFFFFC7
	addq	%rax, %rsi
	shrq	%rsi
	leaq	L_.str.1(%rip), %rdx
	leaq	L_.str.13(%rip), %rcx
	xorl	%eax, %eax
                                        ## kill: def %esi killed %esi killed %rsi
	callq	_mvprintw
	movq	(%r12), %rdi
	callq	_wrefresh
	movq	(%r12), %rdi
	callq	_wgetch
	callq	_endwin
	jmp	LBB4_6
LBB4_4:
	movq	(%r12), %rdi
	callq	_wrefresh
	callq	_endwin
	leaq	L_str(%rip), %rdi
	callq	_puts
	movl	$2, %r14d
LBB4_6:
	movl	%r14d, %eax
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	":"

L_.str.1:                               ## @.str.1
	.asciz	"%s"

L_.str.2:                               ## @.str.2
	.asciz	"SERVER INFO"

L_.str.3:                               ## @.str.3
	.asciz	"                     "

L_.str.4:                               ## @.str.4
	.asciz	"Connecting to..."

L_.str.5:                               ## @.str.5
	.asciz	"Host: "

L_.str.6:                               ## @.str.6
	.asciz	"IP: "

L_.str.7:                               ## @.str.7
	.asciz	"Port: "

L_.str.8:                               ## @.str.8
	.asciz	"Failed To Conenct!"

L_.str.9:                               ## @.str.9
	.asciz	"Connection Sucessful!"

L_.str.10:                              ## @.str.10
	.asciz	"Login"

L_.str.11:                              ## @.str.11
	.asciz	"Username: "

L_.str.12:                              ## @.str.12
	.asciz	"Password: "

L_.str.13:                              ## @.str.13
	.asciz	"Your screen needs to be atleast 40x40. Press Enter to End"

L_.str.15:                              ## @.str.15
	.asciz	"Enter Hostname and Port: "

L_.str.16:                              ## @.str.16
	.asciz	"Welcome to Chess."

L_.str.17:                              ## @.str.17
	.asciz	"The host is not responding because it does not exist or it is offline."

L_.str.18:                              ## @.str.18
	.asciz	"The host's name and/or port does not support a chess server."

	.p2align	4               ## @str
L_str:
	.asciz	"Your terminal does not support color. You need color for this application."


.subsections_via_symbols
