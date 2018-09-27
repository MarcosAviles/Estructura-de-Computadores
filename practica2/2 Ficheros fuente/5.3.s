.section .data
	.macro linea
	 #	.int -1,-2,1,-2
	 #	.int 1,2,-3,-4,1
	 	.int 0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff
	 #	.int 0x80000000,0x80000000,0x80000000,0x80000000
	 #	.int 0xf0000000,0xe0000000,0xe0000000,0xd0000000
	 #	.int -1
	 #	.int 0,-1,-1,-1
	 #	.int 0,-2,-1,-1
	 #	.int 1,-2,-1,-1
	 #	.int 0x08000000,0x08000000,0x08000000,0x08000000
	 #	.int 0xFC000000,0xFC000000,0xfc000000,0xfc000000
	 # 	.int 0xF8000000,0xF8000000,0xf8000000,0xf8000000
	 #	.int 0xF0000000,0xE0000000,0xE0000000,0xF0000000
	.endm
lista:	.irpc i,12345678
		linea
	.endr

longlista:	.int (.-lista)/4
resultado:	.int -1
formato:
	.ascii "suma = %lld = %llx hex\n\0"

.section .text

main:	.global main

	mov    $lista, %ebx
	mov longlista, %ecx
	call suma
	mov %eax, resultado
	mov %edx, resultado+4

	push resultado
	push resultado
	push $formato	
	call printf	
	add $12, %esp

	mov $1, %eax
	mov $0, %ebx
	int $0x80

suma:
	push %edx
	mov $0, %eax
	mov $0, %edx
	mov $0, %esi
	mov $0, %edi

bucle:	
	mov (%ebx, %edx, 4), %edi   #Movemos la instruccion en el registro edi para ahorrarnos trabajo de realizar la operacion (%ebx, %edx, 4).
	cmp $0, %edi 		    #Comparamos si el contenido del registro es positivo o negativo
	js salto		   # saltamos si signo
	add %edi, %eax		    # si el numero es positivo sumamos el elemento iésimo de la lista y lo guardamos en eax
	adc $0, %esi		    # Sumamos los dos operandos y sumamos 1 si hay acarreo
	jmp salto2

salto: 
        add %edi, %eax	      # Si el numero es negativo sumamos el elemento iésimo de la lista y lo guardamos en eax
	adc $-1, %esi		# Sumamos los dos operandos y sumamos 1 si hay acarreo

salto2:
	inc %edx		#Incrementamos el contador i del bucle
	cmp %edx, %ecx
	jne bucle
	mov %esi, %edx	     # despues del bucle movemos lo que hay en esi y lo pasamos a edx
	idiv %ecx	# Idiv realiza la division entera con signo
	pop %edx
	ret
