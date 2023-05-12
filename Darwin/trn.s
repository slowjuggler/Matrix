.global _trn
_trn:
    push %rbp
    push %rbx
    push %r12
    push %r13
    push %r14
    push %r15
    movq %rsp, %rbp
    movq %rdi, %r10
    movq %rsi, %r11
    xorq %r13, %r13
    movq $4, %rax
l1:
    cmpq %r13, %rcx
    je l5
    movq (%r11, %r13, 8), %rdi
    xorq %r14, %r14
    incq %r13
    movq %r13, %r15
    decq %r15
    cmpq %rax, %rcx
    jb l4
l2:
    cmpq %r14, %rdx
    je l1
    movq %r14, %rbx
    subq %rax, %rbx
    cmpq %rax, %rbx
    jb l3
    movq (%r10, %r14, 8), %rsi
    vmovups (%rsi, %r15, 8), %ymm0
    vmovups %ymm0, (%rdi, %r14, 8)
    addq %rax, %r14
    jmp l2
l3:
    vzeroupper
    subq $3, %r14
l4:
    cmpq %r14, %rdx
    je l1
    movq (%r10, %r14, 8), %rsi
    movq (%rsi, %r15, 8), %xmm0
    movq %xmm0, (%rdi, %r14, 8)
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
