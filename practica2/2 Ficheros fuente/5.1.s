.section .data
	.macro linea
	 #	.int 1,1,1,1
	 # 	.int 2,2,2,2
	 	.int 1,2,3,4
	 #	.int -1,-1,-1,-1
	 #	.int 0xffffffff,0xffffffff,0xffffffff,0xffffffff
	 # 	.int 0x08000000,0x08000000,0x08000000,0x08000000
	 #	.int 0x10000000,0x20000000,0x40000000,0x80000000
	.endm
lista:	.irpc i,12345678
		linea
	.endr
longlista:	.int (.-lista)/4
resultado:	.int -1
formato: .ascii "suma = %8u = %08x hex\n\0"

.section .text
main:	.global main

	mov    $lista, %ebx  # El primer elemento de la lista
	mov longlista, %ecx   #Guardamos el tamaño de la lista
	call suma
	mov %eax, resultado  # En eax tenemos el resultado

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
	mov $0, %eax	   # Ponemos a 0 los registros
	mov $0, %edx
	mov $0, %esi
bucle:
	add (%ebx,%esi,4), %eax  #Sumamos el elemento iésimo elemento de la lista y lo almacenamos en eax
	jnc incrementar		 # Salto si no hay acarreo
	inc       %edx		 # Incrementamos el registro edx si hay acarreo

incrementar:
	inc 	  %esi		 # incrementamos el registro esi que es el contador i del bucle for
	cmp  %esi,%ecx		# Comparamos el contador con el tamaño de la lista
	jne bucle		 # Salto si no son iguales

	pop %esi    # recuperamos memoria de la pila
	ret
