#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <string.h>	// para strncmp()/strlen()
#include <sys/time.h>	// para gettimeofday(), struct timeval

char password[]="sacalacabra";
int  passcode  = 7777;

void boom(){
	printf("***************\n");
	printf("*** BOOM!!! ***\n");
	printf("***************\n");
	exit(-1);
}

void defused(){
	printf("*************************\n");
	printf("*** bomba desactivada ***\n");
	printf("*************************\n");
	exit(0);
}


int main(){
#define SIZE 100
	char pass[SIZE];
	int  pasv;
#define TLIM 20
	struct timeval tv1,tv2;	// gettimeofday() secs-usecs

	gettimeofday(&tv1,NULL);
	
	printf("Introduce la contraseña: ");
	fgets(pass,SIZE,stdin);

	for (int i=0; i<strlen(pass); i++) {
		if(pass[i]==toupper(pass[i])){
			pass[i] = tolower(pass[i]);
		}
		else{
			pass[i]= toupper(pass[i]);
		}
	}
	if (strncmp(pass,password,strlen(password)))
	    boom();

	gettimeofday(&tv2,NULL);
	if (tv2.tv_sec - tv1.tv_sec > TLIM)
	    boom();

	printf("Introduce el código: ");
	scanf("%i",&pasv);

	pasv=pasv+5761;
	if (pasv!=passcode) boom();

	gettimeofday(&tv1,NULL);
	if (tv1.tv_sec - tv2.tv_sec > TLIM)
	    boom();

	defused();
}
