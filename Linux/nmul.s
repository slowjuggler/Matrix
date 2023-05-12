.globl nmul
nmul:
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
    vpbroadcastq %xmm0, %ymm0
l1:
    cmpq %r13, %rdx
    je l5
    movq (%r10, %r13, 8), %rdi
    movq (%r11, %r13, 8), %rsi
    xorq %r14, %r14
    incq %r13
    cmpq %rax, %rcx
    jb l4
l2:
    cmpq %r14, %rcx
    je l1
    movq %r14, %rbx
    subq %rdx, %rbx
    cmpq %rax, %rbx
    jb l3
    vmovups (%rdi, %r14, 8), %ymm1
    vmulpd %ymm0, %ymm1, %ymm2
    vmovups %ymm2, (%rsi, %r14, 8)
    addq %rax, %r14
    jmp l2
l3:
    vzeroupper
    subq $3, %r14
l4:
    cmpq %r14, %rcx
    je l1
    movq (%rdi, %r14, 8), %xmm1
    mulsd %xmm0, %xmm1
    movq %xmm1, (%rsi, %r14, 8)
    incq %r14
    jmp l4
l5:
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    popq %rbx
    popq %rbp
    ret
