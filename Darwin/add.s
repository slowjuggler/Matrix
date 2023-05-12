.global _add
_add:
    push %rbp
    push %rbx
    push %r12
    push %r13
    push %r14
    push %r15
	movq %rsp, %rbp
    movq %rdi, %r10
    movq %rsi, %r11
    movq %rdx, %r12
    xorq %r13, %r13
    movq $4, %rax
l1:
    cmpq %r13, %rcx
    je l5
    movq (%r10, %r13, 8), %rdi
    movq (%r11, %r13, 8), %rsi
    movq (%r12, %r13, 8), %rdx
    xorq %r14, %r14
    incq %r13
    cmpq %rax, %r8
    jb l4
l2:
    cmpq %r14, %r8
    je l1
    movq %r14, %rbx
    subq %r8, %rbx
    cmpq %rax, %rbx
    jb l3
    vmovups (%rdi, %r14, 8), %ymm0
    vmovups (%rsi, %r14, 8), %ymm1
    vaddpd %ymm0, %ymm1, %ymm2
    vmovups %ymm2, (%rdx, %r14, 8)
    addq %rax, %r14
    jmp l2
l3:
    vzeroupper
    subq $3, %r14
l4:
    cmpq %r14, %r8
    je l1
    movq (%rdi, %r14, 8), %xmm0
    movq (%rsi, %r14, 8), %xmm1
    addsd %xmm1, %xmm0
    movq %xmm0, (%rdx, %r14, 8)
    incq %r14
    jmp l4
l5:
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp
    ret
