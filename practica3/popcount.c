#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define TEST 0
#define COPY_PASTE_CALC 1

#if ! TEST
	#define NBITS 20
	#define SIZE (1<<NBITS)
	unsigned list[SIZE];
	#define RESULT (SIZE/2)
#else
/*#define SIZE 4
	unsigned list[SIZE]= {0x80000000,0x00100000,0x00000800,0x00000001};
#define RESULT 4
*/
/*#define SIZE 8
unsigned list[SIZE]=
{0x7FFFFFFF,0xFFEFFFFF,0xFFFFF7FF,0xFFFFFFFE,0x01000024,0x00356700,0x8900ac00,0x00BD00EF};
#define RESULT 8
/*--------------------------*/
#define SIZE 8
unsigned list[SIZE]=
{0x0,0x10204080,0x3590AC06,0x70B0D0E0,0xFFFFFFFF,0x12345678,0x9ABCDEF0,0xCAFEBEEF};
#define RESULT 2
/*--------------------------*/
#endif

#define WSIZE 8*sizeof(int)
int resultado=0;

int popcount1(unsigned *array, int len){
	int i,j;
	unsigned int x;
	int result = 0;
	for(i=0;i<len;i++){
		x=array[i];
		for (j = 0; j < WSIZE; j++) {
			unsigned mask = 1 << j;
			result += (x & mask) != 0;
		}
	}
	return result;
}


int popcount2(unsigned *array, int len){
	int i;
	unsigned int x;
	int result = 0;
	for(i=0;i<len;i++){
		x=array[i];
		do {
			result += (x & 0x1)!=0;;
			x >>= 1;
		} while (x);
	}
	return result;
}

int popcount3(unsigned *array, int len){
	int i;
	unsigned int x;
	int result = 0;
	for(i=0;i<len;i++){
		x=array[i];
		asm("ini: \n"
		"shr $1, %[x]\n"
		"adc $0,%[result]\n"
		"cmp $0,%[x]\n"
		"jne ini\n"
		:[x]"+r"(x),[result]"+r"(result)
		:
		:
		);
	}
	return result;
}


int popcount4(unsigned *array, int len){
	long val;
	long result=0;
	int i,j;
	unsigned int x;
	for(i=0;i<len;i++){
		x=array[i];
		val=0;
		for(j=0;j<8;j++){
			val+=x &0x01010101;
			x>>=1;
		}
		val+=(val>>16);
		val+=(val>>8);
		result += val &0xFF;
	}		
	return result;
}


int popcount5(unsigned *array, int len){
	int i;
	int val, result = 0;
	int SSE_mask[] = { 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f };
	int SSE_LUTb[] = { 0x02010100, 0x03020201, 0x03020201, 0x04030302 };
	if (len & 0x3){
		printf("leyendo 128b pero (len) no es múltiplo de 4?n");
	}
	for (i = 0; i < len; i += 4) {
		asm("movdqu       %[x], %%xmm0 \n"
			"movdqa %%xmm0, %%xmm1 \n"
			"movdqu   %[m], %%xmm6 \n"
			"psrlw $4,      %%xmm1 \n"
			"pand %%xmm6, %%xmm0 \n"
			"pand %%xmm6, %%xmm1 \n"
			"movdqu %[l], %%xmm2 \n"
			"movdqa %%xmm2, %%xmm3 \n"
			"pshufb %%xmm0, %%xmm2 \n"
			"pshufb %%xmm1, %%xmm3 \n"
			"paddb %%xmm2, %%xmm3 \n"
			"pxor %%xmm0, %%xmm0 \n"
			"psadbw %%xmm0, %%xmm3 \n"
			"movhlps %%xmm3, %%xmm0 \n"
			"paddd %%xmm3, %%xmm0 \n"
			"movd %%xmm0, %[val] \n"
			
			: [val]"=r" (val)
			: [x] "m" (array[i]),
			[m] "m" (SSE_mask[0]),
			[l] "m" (SSE_LUTb[0])
		);
		result += val;
	}
return result;
}


int popcount6(unsigned *array, int len){
	int i;
	unsigned x;
	int val, result = 0;
	for (i = 0; i < len; i++) {
		x = array[i];
		asm("popcnt %[x], %[val] \n"
			: [val]"=r"(val)
			: [x] "r" (x)
		);
	result += val;
	}
	return result;
}


int popcount7(unsigned *array, int len){
	int i;
	unsigned x1, x2;
	int val, result = 0;
	if (len & 0x1){
		printf("Leer 64b y (len) es impar?n");
	}
	for (i = 0; i < len; i += 2) {
		x1 = array[i];
		x2 = array[i + 1];
		asm("popcnt %[x1], %[val] \n"
			"popcnt %[x2], %%edi \n"
			"add %%edi, %[val] \n"
			: [val]"=r"(val)
			: [x1] "r" (x1),
			[x2] "r" (x2)
			: "edi"
		);
		result += val;
	}
	return result;
}


void crono(int (*func)(), char *msg){

	struct timeval tv1,tv2; 
	long tv_usecs;
	gettimeofday(&tv1,NULL);
	resultado = func(list, SIZE);
	gettimeofday(&tv2,NULL);
	tv_usecs=(tv2.tv_sec -tv1.tv_sec)*1E6+
		(tv2.tv_usec-tv1.tv_usec);

	#if ! COPY_PASTE_CALC
		printf("resultado = %d\t", resultado);
		printf("%s:%9ld us\n", msg, tv_usecs);
	#else
		printf("%6ld\n",tv_usecs);
	#endif
}

int main(){
	#if ! TEST
	int i;

	for(i=0; i<SIZE; i++)// se queda en cache
		list[i]=i;
	#endif
crono(popcount1, "popcount1 ()");
crono(popcount2, "popcount2 ()");
crono(popcount3, "popcount3 ()");
crono(popcount4, "popcount4 ()");
crono(popcount5, "popcount5 ()");
crono(popcount6, "popcount6 ()");
crono(popcount7, "popcount7 ()");
#if ! COPY_PASTE_CALC
	printf("Resultado:%d\n",RESULT);
#endif
	exit(0);
}
