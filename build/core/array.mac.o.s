.globl sizeZintegerArr
.type sizeZintegerArr, @function
sizeZintegerArr:
push rbp
mov rbp, rsp
sub rsp, 8
movq [rbp-8], rdi
lea rbx, [rbp-8]
movq rdi, [rbx]
call _internal_get_sizeZintegerArr
movq rbx, rax
movq rax, rbx
leave
ret
leave
ret
.globl sizeZbyteArr
.type sizeZbyteArr, @function
sizeZbyteArr:
push rbp
mov rbp, rsp
sub rsp, 8
movq [rbp-8], rdi
lea rbx, [rbp-8]
movq rdi, [rbx]
call _internal_get_sizeZbyteArr
movq rbx, rax
movq rax, rbx
leave
ret
leave
ret
.globl sizeZcharArr
.type sizeZcharArr, @function
sizeZcharArr:
push rbp
mov rbp, rsp
sub rsp, 8
movq [rbp-8], rdi
lea rbx, [rbp-8]
movq rdi, [rbx]
call _internal_get_sizeZcharArr
movq rbx, rax
movq rax, rbx
leave
ret
leave
ret
.globl sizeZfloatArr
.type sizeZfloatArr, @function
sizeZfloatArr:
push rbp
mov rbp, rsp
sub rsp, 8
movq [rbp-8], rdi
lea rbx, [rbp-8]
movq rdi, [rbx]
call _internal_get_sizeZfloatArr
movq rbx, rax
movq rax, rbx
leave
ret
leave
ret
.globl sizeZboolArr
.type sizeZboolArr, @function
sizeZboolArr:
push rbp
mov rbp, rsp
sub rsp, 8
movq [rbp-8], rdi
lea rbx, [rbp-8]
movq rdi, [rbx]
call _internal_get_sizeZboolArr
movq rbx, rax
movq rax, rbx
leave
ret
leave
ret
.globl sortZintegerArr
.type sortZintegerArr, @function
sortZintegerArr:
push rbp
mov rbp, rsp
sub rsp, 32
movq [rbp-8], rdi
movq [rbp-16], 0
lea rbx, [rbp-8]
movq rdi, [rbx]
call sizeZintegerArr
movq [rbp-24], rax
.LC00000:
movq rbx, [rbp-24]
movq r10, 0
cmp rbx, r10
jle .LC00001
movq rbx, 0
movq [rbp-16], rbx
.LC00002:
movq rbx, [rbp-16]
movq r10, [rbp-24]
movq r11, 1
subq r10, r11
cmp rbx, r10
jge .LC00003
movq r10, [rbp-16]
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
call array_get_element
movq rbx, rax
movq r11, [rbp-16]
movq r12, 1
addq r11, r12
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r11
call array_get_element
movq r10, rax
cmp rbx, r10
jg .LC00004
jmp .LC00005
.LC00004:
movq r10, [rbp-16]
movq r11, 1
addq r10, r11
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
call array_get_element
movq rbx, rax
movq [rbp-32], rbx
movq r10, [rbp-16]
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
call array_get_element
movq rbx, rax
movq r10, [rbp-16]
movq r11, 1
addq r10, r11
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
movq rdx, rbx
call array_set_element
movq rbx, [rbp-32]
movq r10, [rbp-16]
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
movq rdx, rbx
call array_set_element
.LC00005:
movq rbx, [rbp-16]
movq r10, 1
addq rbx, r10
movq [rbp-16], rbx
jmp .LC00002
.LC00003:
movq rbx, [rbp-24]
movq r10, 1
subq rbx, r10
movq [rbp-24], rbx
jmp .LC00000
.LC00001:
leave
ret
.globl sortZbyteArr
.type sortZbyteArr, @function
sortZbyteArr:
push rbp
mov rbp, rsp
sub rsp, 32
movq [rbp-8], rdi
movq [rbp-16], 0
lea rbx, [rbp-8]
movq rdi, [rbx]
call sizeZbyteArr
movq [rbp-24], rax
.LC00006:
movq rbx, [rbp-24]
movq r10, 0
cmp rbx, r10
jle .LC00007
movq rbx, 0
movq [rbp-16], rbx
.LC00008:
movq rbx, [rbp-16]
movq r10, [rbp-24]
movq r11, 1
subq r10, r11
cmp rbx, r10
jge .LC00009
movq r10, [rbp-16]
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
call array_get_element_c
mov bl, al
movq r11, [rbp-16]
movq r12, 1
addq r11, r12
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r11
call array_get_element_c
mov r10b, al
cmp bl, r10b
ja .LC00010
jmp .LC00011
.LC00010:
movq r10, [rbp-16]
movq r11, 1
addq r10, r11
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
call array_get_element_c
mov bl, al
mov byte ptr [rbp-32], bl
movq r10, [rbp-16]
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
call array_get_element_c
mov bl, al
movq r10, [rbp-16]
movq r11, 1
addq r10, r11
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
movq rdx, rbx
call array_set_element_c
mov bl, [rbp-32]
movq r10, [rbp-16]
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
movq rdx, rbx
call array_set_element_c
.LC00011:
movq rbx, [rbp-16]
movq r10, 1
addq rbx, r10
movq [rbp-16], rbx
jmp .LC00008
.LC00009:
movq rbx, [rbp-24]
movq r10, 1
subq rbx, r10
movq [rbp-24], rbx
jmp .LC00006
.LC00007:
leave
ret
.globl sortZcharArr
.type sortZcharArr, @function
sortZcharArr:
push rbp
mov rbp, rsp
sub rsp, 48
movq [rbp-8], rdi
movq [rbp-16], 0
lea rbx, [rbp-8]
movq rdi, [rbx]
call sizeZcharArr
movq [rbp-24], rax
.LC00012:
movq rbx, [rbp-24]
movq r10, 0
cmp rbx, r10
jle .LC00013
movq rbx, 0
movq [rbp-16], rbx
.LC00014:
movq rbx, [rbp-16]
movq r10, [rbp-24]
movq r11, 1
subq r10, r11
cmp rbx, r10
jge .LC00015
movq r10, [rbp-16]
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
call array_get_element_c
mov bl, al
mov byte ptr [rbp-40], bl
movq r10, [rbp-16]
movq r11, 1
addq r10, r11
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
call array_get_element_c
mov bl, al
mov byte ptr [rbp-48], bl
mov bl, [rbp-40]
movq rdi, rbx
call to_byteZchar
movq rbx, rax
push rbx
mov bl, [rbp-48]
movq rdi, rbx
call to_byteZchar
pop rbx
movq r10, rax
cmp bl, r10b
ja .LC00016
jmp .LC00017
.LC00016:
movq r10, [rbp-16]
movq r11, 1
addq r10, r11
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
call array_get_element_c
mov bl, al
mov byte ptr [rbp-32], bl
movq r10, [rbp-16]
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
call array_get_element_c
mov bl, al
movq r10, [rbp-16]
movq r11, 1
addq r10, r11
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
movq rdx, rbx
call array_set_element_c
mov bl, [rbp-32]
movq r10, [rbp-16]
lea rdi, [rbp-8]
mov rdi, [rdi]
movq rsi, r10
movq rdx, rbx
call array_set_element_c
.LC00017:
movq rbx, [rbp-16]
movq r10, 1
addq rbx, r10
movq [rbp-16], rbx
jmp .LC00014
.LC00015:
movq rbx, [rbp-24]
movq r10, 1
subq rbx, r10
movq [rbp-24], rbx
jmp .LC00012
.LC00013:
leave
ret
