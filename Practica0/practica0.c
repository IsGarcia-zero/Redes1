// SI yo quiero poner la letra en binario es
#include<stdio.h>
int main(){
	unsigned char contador = 0, i = 1;
	char letra;
	printf("Introdusca un caracter: \n");
	scanf("%c", &letra);
	for(; i != 0; i <<= 1){
		if(letra&i){
			contador++;
			printf("1");
		}
		printf("0");
	}
	printf("\n");
	printf("Tiene %u bits encendidos\n", contador);
	return 0;
}
