	.file	"mm.c"
	.text
	.type	search_free_list, @function
search_free_list:
	pushq	%rbx
	movq	%rdi, %rbx
	call	mem_heap_lo
	movq	(%rax), %rax
	jmp	.L2
.L6:
	movq	8(%rax), %rax
.L2:
	testq	%rax, %rax
	je	.L1
	movq	(%rax), %rdx
	andq	$-8, %rdx
	cmpq	%rbx, %rdx
	jb	.L6
.L1:
	popq	%rbx
	ret
	.size	search_free_list, .-search_free_list
	.type	insert_free_block, @function
insert_free_block:
	pushq	%rbx
	movq	%rdi, %rbx
	call	mem_heap_lo
	movq	(%rax), %rax
	movq	%rax, 8(%rbx)
	testq	%rax, %rax
	je	.L8
	movq	%rbx, 16(%rax)
.L8:
	movq	$0, 16(%rbx)
	call	mem_heap_lo
	movq	%rbx, (%rax)
	popq	%rbx
	ret
	.size	insert_free_block, .-insert_free_block
	.type	remove_free_block, @function
remove_free_block:
	pushq	%r12
	pushq	%rbp
	pushq	%rbx
	movq	%rdi, %rbx
	movq	8(%rdi), %rbp
	movq	16(%rdi), %r12
	testq	%rbp, %rbp
	je	.L11
	movq	%r12, 16(%rbp)
.L11:
	call	mem_heap_lo
	cmpq	%rbx, (%rax)
	je	.L15
	movq	%rbp, 8(%r12)
.L10:
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L15:
	call	mem_heap_lo
	movq	%rbp, (%rax)
	jmp	.L10
	.size	remove_free_block, .-remove_free_block
	.type	coalesce_free_block, @function
coalesce_free_block:
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbp
	pushq	%rbx
	subq	$8, %rsp
	movq	%rdi, %r13
	movq	(%rdi), %r14
	andq	$-8, %r14
	movq	%r14, %r12
	movq	%rdi, %rbx
	jmp	.L17
.L18:
	movq	-8(%rbx), %rbp
	andq	$-8, %rbp
	movslq	%ebp, %rax
	subq	%rax, %rbx
	movq	%rbx, %rdi
	call	remove_free_block
	addq	%rbp, %r12
.L17:
	testb	$2, (%rbx)
	je	.L18
	movslq	%r14d, %rbp
	addq	%r13, %rbp
	jmp	.L19
.L20:
	andq	$-8, %rax
	movq	%rax, %r15
	movq	%rbp, %rdi
	call	remove_free_block
	addq	%r15, %r12
	movslq	%r15d, %rax
	addq	%rax, %rbp
.L19:
	movq	0(%rbp), %rax
	testb	$1, %al
	je	.L20
	cmpq	%r14, %r12
	jne	.L23
.L16:
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
.L23:
	movq	%r13, %rdi
	call	remove_free_block
	orq	$2, %r12
	movq	%r12, (%rbx)
	movq	%r12, -8(%rbp)
	movq	%rbx, %rdi
	call	insert_free_block
	jmp	.L16
	.size	coalesce_free_block, .-coalesce_free_block
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"ERROR: mem_sbrk failed in request_more_space"
	.text
	.type	request_more_space, @function
request_more_space:
	pushq	%rbp
	pushq	%rbx
	subq	$8, %rsp
	movq	%rdi, %rbx
	call	mem_pagesize
	movq	%rax, %rcx
	leaq	-1(%rax,%rbx), %rax
	movl	$0, %edx
	divq	%rcx
	imulq	%rax, %rcx
	movq	%rcx, %rbx
	movq	%rcx, %rdi
	call	mem_sbrk
	cmpq	$-1, %rax
	je	.L27
	leaq	-8(%rax), %rbp
	movq	-8(%rax), %rdx
	andl	$2, %edx
	orq	%rbx, %rdx
	movq	%rdx, -8(%rax)
	leal	-8(%rbx), %eax
	cltq
	movq	%rdx, 0(%rbp,%rax)
	movslq	%ebx, %rcx
	movq	$1, 0(%rbp,%rcx)
	movq	%rbp, %rdi
	call	insert_free_block
	movq	%rbp, %rdi
	call	coalesce_free_block
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
.L27:
	movl	$.LC0, %edi
	call	puts
	movl	$0, %edi
	call	exit
	.size	request_more_space, .-request_more_space
	.section	.rodata.str1.8
	.align 8
.LC1:
	.string	"ERROR: mem_sbrk failed in mm_init, returning %p\n"
	.text
	.globl	mm_init
	.type	mm_init, @function
mm_init:
	pushq	%rbx
	movl	$48, %edi
	call	mem_sbrk
	cmpq	$-1, %rax
	je	.L31
	call	mem_heap_lo
	leaq	8(%rax), %rbx
	movq	$34, 8(%rax)
	movq	$0, 16(%rax)
	movq	$0, 24(%rax)
	movq	$34, 32(%rax)
	call	mem_heap_hi
	movq	$1, -7(%rax)
	call	mem_heap_lo
	movq	%rbx, (%rax)
	movl	$0, %eax
	popq	%rbx
	ret
.L31:
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
	.size	mm_init, .-mm_init
	.globl	mm_malloc
	.type	mm_malloc, @function
mm_malloc:
	testq	%rdi, %rdi
	je	.L41
	pushq	%r12
	pushq	%rbp
	pushq	%rbx
	leaq	8(%rdi), %rax
	cmpq	$32, %rax
	jbe	.L42
	leaq	15(%rdi), %rbp
	andq	$-8, %rbp
.L34:
	movq	%rbp, %rdi
	call	search_free_list
	movq	%rax, %rbx
	jmp	.L35
.L42:
	movl	$32, %ebp
	jmp	.L34
.L36:
	movl	$32768, %edi
	call	request_more_space
	movq	%rbp, %rdi
	call	search_free_list
	movq	%rax, %rbx
.L35:
	testq	%rbx, %rbx
	je	.L36
	movq	(%rbx), %r12
	andq	$-8, %r12
	movq	%r12, %rax
	subq	%rbp, %rax
	cmpq	$31, %rax
	ja	.L37
	movslq	%r12d, %rax
	orq	$2, (%rbx,%rax)
.L38:
	movq	%rbx, %rdi
	call	remove_free_block
	testb	$2, (%rbx)
	je	.L39
	orq	$3, %r12
	movq	%r12, (%rbx)
.L40:
	leaq	8(%rbx), %rax
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L37:
	movslq	%ebp, %rdi
	addq	%rbx, %rdi
	movq	%rax, %rdx
	orq	$2, %rdx
	movq	%rdx, (%rdi)
	andq	$-2, %rdx
	movq	%rdx, (%rdi)
	subl	$8, %eax
	cltq
	movq	%rdx, (%rdi,%rax)
	call	insert_free_block
	movq	%rbp, %r12
	jmp	.L38
.L39:
	orq	$1, %r12
	movq	%r12, (%rbx)
	jmp	.L40
.L41:
	movl	$0, %eax
	ret
	.size	mm_malloc, .-mm_malloc
	.globl	mm_free
	.type	mm_free, @function
mm_free:
	pushq	%rbx
	leaq	-8(%rdi), %rbx
	movq	-8(%rdi), %rdx
	movq	%rdx, %rax
	andq	$-8, %rax
	movslq	%eax, %rcx
	andq	$-2, %rdx
	movq	%rdx, -8(%rdi)
	subl	$8, %eax
	cltq
	movq	%rdx, -8(%rdi,%rax)
	andq	$-3, (%rdi,%rcx)
	movq	%rbx, %rdi
	call	insert_free_block
	movq	%rbx, %rdi
	call	coalesce_free_block
	popq	%rbx
	ret
	.size	mm_free, .-mm_free
	.globl	mm_check
	.type	mm_check, @function
mm_check:
	movl	$0, %eax
	ret
	.size	mm_check, .-mm_check
	.ident	"GCC: (GNU) 11.4.1 20230605 (Red Hat 11.4.1-2)"
	.section	.note.GNU-stack,"",@progbits
