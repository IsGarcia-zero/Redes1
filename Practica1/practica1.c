#include<stdio.h>
int main(){
	unsigned char i = 0;
    int checksum = 0;
	unsigned char T[] = {0x45, 0x00, 0x01, 0xe2, 0xd7, 0xdb, 0x40, 0x00, 0x80, 0x06, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x43, 0x94, 0xcc, 0x3a, 0xdd};
	unsigned char T2[] = {0x45, 0x00, 0x01, 0x9c, 0xd7, 0xde, 0x40, 0x00, 0x80, 0x06, 0x88, 0x9d, 0xc0, 0xa8, 0x01, 0x43, 0x94, 0xcc, 0x3a, 0xdd};
	printf("Obtenemos los numeros con los cuales operar = 0x%.2x 0x%.2x \n", T[10], T[11]);
	for(; i <= 18; i+=2)
		checksum += T[i]<<8 | T[i+1];
	printf("El resultado de la operacion: 0x%.2x \n", checksum);
	printf("El checksum es de: %.4X \n", ~((checksum >> 16) + (checksum & 0xFFFF)) & 0xFFFF);
	printf("Para el valor del segundo inciso: \n");
	checksum = 0;
	i = 0;
	for(; i <= 18; i+=2)
		checksum += T2[i]<<8 | T2[i+1];
	printf("El resultado de la operacion: 0x%.2x \n", checksum);
	printf("El checksum es de: %.4X \n", ~((checksum >> 16) + (checksum & 0xFFFF)) & 0xFFFF);
	if (~((checksum >> 16) + (checksum & 0xFFFF)) & 0xFFFF == 0x0000)
		printf("El checksum es correcto \n");
	else{
		printf("El checksum es incorrecto \n");	
		checksum = 0;
		i = 0;
		T2[10] = 0x00;
		T2[11] = 0x00;
		for(; i <= 18; i+=2)
			checksum += T2[i]<<8 | T2[i+1];
		printf("El resultado de la operacion: 0x%.2x \n", checksum);
		printf("El checksum es de: %.4X \n", ~((checksum >> 16) + (checksum & 0xFFFF)) & 0xFFFF);
		printf("Comprobamos el cheksum \n");
		T2[10] = 0x8f;
		T2[11] = 0xe8;
		checksum = 0;
		i = 0;
		for(; i <= 18; i+=2)
			checksum += T2[i]<<8 | T2[i+1];
		printf("El resultado de la operacion: 0x%.2x \n", checksum);
		printf("Comprobamos que es el checksum anterior es correcto: %.4X \n", ~((checksum >> 16) + (checksum & 0xFFFF)) & 0xFFFF);
	}
	return 0;

}

