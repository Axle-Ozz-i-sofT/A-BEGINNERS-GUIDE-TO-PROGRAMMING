	.file	"main.c"
	.text
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC0:
	.ascii "Please enter your name: \0"
.LC1:
	.ascii "%s\0"
	.align 8
.LC2:
	.ascii "Hello %s. Can you please enter 2 numbers: \12\0"
.LC3:
	.ascii "%d %d\0"
	.align 8
.LC4:
	.ascii "\12%s the addition of two numbers is : %d\12\0"
	.align 8
.LC5:
	.ascii "Press the [Enter] key to continue...\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	addq	$-128, %rsp
	.seh_stackalloc	128
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	call	__main
	leaq	.LC0(%rip), %rcx
	call	printf
	leaq	-80(%rbp), %rax
	movq	%rax, %rdx
	leaq	.LC1(%rip), %rcx
	call	scanf
	leaq	-80(%rbp), %rax
	movq	%rax, %rdx
	leaq	.LC2(%rip), %rcx
	call	printf
	leaq	-88(%rbp), %rdx
	leaq	-84(%rbp), %rax
	movq	%rdx, %r8
	movq	%rax, %rdx
	leaq	.LC3(%rip), %rcx
	call	scanf
	movl	-88(%rbp), %edx
	movl	-84(%rbp), %eax
	movl	%eax, %ecx
	call	sum
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %edx
	leaq	-80(%rbp), %rax
	movl	%edx, %r8d
	movq	%rax, %rdx
	leaq	.LC4(%rip), %rcx
	call	printf
	leaq	.LC5(%rip), %rcx
	call	printf
	call	getch
	movl	$0, %eax
	subq	$-128, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	sum
	.def	sum;	.scl	2;	.type	32;	.endef
	.seh_proc	sum
sum:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$16, %rsp
	.seh_stackalloc	16
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movl	%edx, 24(%rbp)
	movl	16(%rbp), %edx
	movl	24(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (tdm64-1) 9.2.0"
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	scanf;	.scl	2;	.type	32;	.endef
	.def	getch;	.scl	2;	.type	32;	.endef
