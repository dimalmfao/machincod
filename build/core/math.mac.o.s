.globl powZintegerZinteger
.type powZintegerZinteger, @function
powZintegerZinteger:
push rbp
mov rbp, rsp
sub rsp, 24
movq [rbp-8], rdi
movq [rbp-16], rsi
movq [rbp-24], 1
.LC00000:
movq rbx, [rbp-16]
movq r10, 0
cmp rbx, r10
jle .LC00001
movq rbx, [rbp-24]
movq r10, [rbp-8]
imul rbx, r10
movq [rbp-24], rbx
movq rbx, [rbp-16]
movq r10, 1
subq rbx, r10
movq [rbp-16], rbx
jmp .LC00000
.LC00001:
movq rbx, [rbp-24]
movq rax, rbx
leave
ret
leave
ret
.globl absZinteger
.type absZinteger, @function
absZinteger:
push rbp
mov rbp, rsp
sub rsp, 8
movq [rbp-8], rdi
movq rbx, [rbp-8]
movq r10, 0
cmp rbx, r10
jl .LC00002
jmp .LC00003
.LC00002:
movq rbx, [rbp-8]
movq r10, -1
imul rbx, r10
movq [rbp-8], rbx
.LC00003:
movq rbx, [rbp-8]
movq rax, rbx
leave
ret
leave
ret
.globl factorialZinteger
.type factorialZinteger, @function
factorialZinteger:
push rbp
mov rbp, rsp
sub rsp, 24
movq [rbp-8], rdi
movq [rbp-16], 1
movq [rbp-24], 1
.LC00004:
movq rbx, [rbp-24]
movq r10, [rbp-8]
cmp rbx, r10
jg .LC00005
movq rbx, [rbp-16]
movq r10, [rbp-24]
imul rbx, r10
movq [rbp-16], rbx
movq rbx, [rbp-24]
movq r10, 1
addq rbx, r10
movq [rbp-24], rbx
jmp .LC00004
.LC00005:
movq rbx, [rbp-16]
movq rax, rbx
leave
ret
leave
ret
