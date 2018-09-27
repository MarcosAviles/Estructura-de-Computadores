.section .data
lista:		.int 1,2,10,  1,2,0b10,  1,2,0x10
longlista:	.int (.-lista)/4
resultado:	.int -1
formato: .ascii "suma = %8u = %08x hex\n\0"

.section .text
main:	.global main

	mov    $lista, %ebx
	mov longlista, %ecx
	call suma
	mov %eax, resultado

	push resultado
	push resultado
	push $formato
	call printf
	add $12, %esp

	mov $1, %eax
	mov $0, %ebx
	int $0x80

suma:
	push %esi
	mov $0, %eax
	mov $0, %edx
	mov $0, %esi
bucle:
	add (%ebx,%esi,4), %eax
	mov %eax, resultado
	jnc incrementar
	inc       %edx
incrementar:
	inc 	  %esi

	push %eax
	push %ecx
	push %edx

	push resultado
	push resultado
	push $formato

	call printf
	add $12, %esp

	pop %edx
	pop %ecx
	pop %eax


	cmp  %esi,%ecx
	jne bucle

	pop %esi
	ret
