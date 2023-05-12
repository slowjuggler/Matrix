.globl mul
mul:
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
    je l6
    xorq %r14, %r14
    movq (%r10, %r13, 8), %rdi
    movq (%r12, %r13, 8), %rdx
    incq %r13
l2:
    cmpq %r14, %r9
    je l1
    vpbroadcastq (%rdi, %r14, 8), %ymm0
    movq (%r11, %r14, 8), %rsi
    xorq %r15, %r15
    incq %r14
    cmpq %rax, %r8
    jb l5
l3:
    cmpq %r15, %r8
    je l2
    movq %r15, %rbx
    subq %r8, %rbx
    incq %rbx
    cmpq %rax, %rbx
    jb l4
    vmovups (%rsi, %r15, 8), %ymm1
    vmovups (%rdx, %r15, 8), %ymm3
    vfmadd231pd %ymm0, %ymm1, %ymm3
    vmovups %ymm3, (%rdx, %r15, 8)
    addq %rax, %r15
    jmp l3
l4:
    vzeroupper
    subq $3, %r15
l5:
    cmpq %r15, %r8
    je l2
    movsd (%rsi, %r15, 8), %xmm1
    mulsd %xmm0, %xmm1
    movsd (%rdx, %r15, 8), %xmm2
    addsd %xmm1, %xmm2
    movsd %xmm2, (%rdx, %r15, 8)
    incq %r15
    jmp l5
l6:
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp
    ret
