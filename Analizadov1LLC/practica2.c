#include<stdio.h>
#include <stdlib.h>
void analizarTrama(unsigned char *T);
void leerTrama(unsigned char *);
void analizaARP(unsigned char *);
void decimal(char c, unsigned char *);
int main(){
	printf("Integrantes: \n");
	printf("Hernandez Vergara Eduardo\nRojas Cruz Jose Angel");
    FILE * ptr;
    char c;
	unsigned char hex = 0, ct = 0, p = 0, n = 0, i = 0;
	unsigned char nbytet[36];
	ptr = fopen("tramaP.txt", "r");
    while(c != EOF){
		c = fgetc(ptr);
		if(c == '}'){
			nbytet[p] = ct;
			ct = 0;
			p++;
		}
		if(hex == 2){
			hex = 0;
			ct++;//Contar cantidad de bytes
		}
		if(hex == 1){
			hex++;
		}
			
		if(c == 'x')
			hex = 1;
	}
	fclose(ptr);
	p = 0;
	c = 0;
	ptr = fopen("tramaP.txt", "r");
		unsigned char *trama = (char*)(malloc(sizeof(char)*nbytet[0]));
		while(c != EOF){
			c = fgetc(ptr);
			if(c == '}'){
                leerTrama(trama);
                free(trama);
                i++;
                trama = (char*)(malloc(sizeof(char)*nbytet[i]));
				p = 0;
			}	
			if(hex == 2){
				decimal(c, &n);
				trama[p] = n;
				n = 0;
				hex = 0;
				p++;
			}
			if(hex == 1){
				decimal(c, &n);
				n *= 16;
				hex++;
			}
				
			if(c == 'x')
				hex = 1;
		}
	fclose(ptr);
    
    return 0;
}

void leerTrama(unsigned char * T){
    printf("\nCabecera ethernet \n");
    unsigned short tot = T[12] << 8 | T[13];
    printf("MAC DESTINO %.2x: %.2x: %.2x: %.2x: %.2x: %.2x\n", T[0], T[1], T[2], T[3], T[4], T[5]);
    printf("MAC ORIGEN %.2x: %.2x: %.2x: %.2x: %.2x: %.2x\n", T[6], T[7], T[8], T[9], T[10], T[11]);
    if (tot < 1500){
        printf("Tamano de la cabecera LLC: %d bytes \n", tot);
        analizarTrama(T);
    }else{
        if (tot == 2048){
            printf("TIPO IP\n");// analiza IP
        }else if (tot == 2054){
            printf("TIPO ARP\n");// analiza ARP
            analizaARP(T);
        }else{
            printf("TIPO: %.2x%.2x", T[12], T[13]);
        }
    }
}

void analizarTrama(unsigned char *T){
    char ss[][5] = {"RR", "RNR", "REJ", "SREJ"};
    char uc[][5] = {"UI", "SIM", "-", "SARM", "UP", "-", "-", "SABM", "DISC",
    "-", "-", "SARME", "--", "-", "-", "SABME", "SNRM", "--", "-", "RSET", "-",
    "-", "-", "XID", "-", "-", "-", "SNRME"};
    char ur[][5] = {"UI", "RIM", "-", "DM", "-", "-", "-", "-", "RD",
    "-", "-", "--", "UA", "-", "-", "--", "--", "FRMR", "-", "--", "-",
    "-", "-", "XID", "-", "-", "-", "--"};
    printf("TIPO: %.2x %.2x\n", T[16], T[17]);
    switch (T[16]&3){
    case 0:
        if (T[17]&1){
            if (T[15]&1){
                printf("TIPO: T-I. N(s) = %d, N(r)=%d 1-f\n", T[16]>>1, T[17]>>1);
            }else{
                printf("TIPO: T-I. N(s) = %d, N(r)=%d 1-p\n", T[16]>>1, T[17]>>1);
            }
        }else{
            if (T[15]&1){
                printf("TIPO: T-I. N(s) = %d, N(r)=%d 0-f\n", T[16]>>1, T[17]>>1);
            }else{
                printf("TIPO: T-I. N(s) = %d, N(r)=%d 0-p\n", T[16]>>1, T[17]>>1);
            }
        }
        break;
    case 1:
        printf("t-S, S = %s\n", ss[(T[16]>>2)&3]);
        if (T[17]&1){
            if (T[15]&1){
                printf("TIPO: T-S. N(s) = -, N(r)=%d 1-f\n", T[17]>>1);
            }
            else{
                printf("TIPO: T-S. N(s) = -, N(r)=%d 1-p\n", T[17]>>1);
            }
        }else{
            if (T[15]&1){
                printf("TIPO: T-S. N(s) = -, N(r)=%d 0-f\n", T[17]>>1);
            }
            else{
                printf("TIPO: T-S. N(s) = -, N(r)=%d 0-p\n", T[17]>>1);
            }
        }
        break;
    case 2:
        if (T[17]&1){
            if (T[15]&1){
                printf("TIPO: T-I. N(s) = %d, N(r)=%d 1-f\n", T[16]>>1, T[17]>>1);
            }else{
                printf("TIPO: T-I. N(s) = %d, N(r)=%d 1-p\n", T[16]>>1, T[17]>>1);
            }
        }else{
            if (T[15]&1){
                printf("TIPO: T-I. N(s) = %d, N(r)=%d 0-f\n", T[16]>>1, T[17]>>1);
            }else{
                printf("TIPO: T-I. N(s) = %d, N(r)=%d 0-p\n", T[16]>>1, T[17]>>1);
            }
        }
        break;
    case 3: 
        if (T[16]&16){
            if (T[15]&1){
                printf("T-U %s 1-f\n", ur[(T[16]>>2&3)|(T[16]>>3&28)]);
            }else{
                printf("T-U %s 1-p\n", uc[(T[16]>>2&3)|(T[16]>>3&28)]);
            }
        }else{
            if (T[15]&1){
                printf("T-U %s 0-f\n", ur[(T[16]>>2&3)|(T[16]>>3&28)]);
            }else{
                printf("T-U %s 0-p\n", uc[(T[16]>>2&3)|(T[16]>>3&28)]);
            }
        }
        break;
    }
}
void analizaARP(unsigned char *T){
    if(T[14]<<8 | T[15] == 1){
        printf("TIPO: Ethernet\n");
    }else if(T[14]<<8 | T[15] == 6){
        printf("IEEE 80.2 LAN\n");
    }else{
        printf("Otro: %d\n", T[14]<<8 |T[15]);
    }
    // Tipo de direccion de Protocolo
    if (T[16]<<8 | T[17] == 0x0806){
        printf("TIPO: iPv4\n");
    }else{
        printf("TIPO: %.2x, %.2x\n", T[16], T[17]);
    }
    //Tama??o de la MAC
    printf("Tama??o MAC: %d bytes\n", T[18]);
    //Tama??o de la Direccion IP
    printf("Tama??o IP: %d bytes\n", T[19]);
    //Op Code
    if (T[20]<<8 | T[21] == 1){
        printf("Op Code: ARP Request\n");
    }else if (T[20]<<8 | T[21] == 2){
        printf("Op Code: ARP Reply\n");
    }else{
        printf("Otro: %d\n", T[20]<<8 | T[21]);
    }
    printf("Direccion MAC origen: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", T[22], T[23], T[24], T[25], T[26], T[27]);
    printf("Direccion IP origen: %d.%d.%d.%d\n", T[28], T[29], T[30], T[31]);
    printf("Direccion MAC destino: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", T[32], T[33], T[34], T[35], T[36], T[37]);
    printf("Direccion IP destino: %d.%d.%d.%d\n", T[38], T[39], T[40], T[41]);
    
}

void decimal(char c, unsigned char *n){
	switch(c){
		case '1':
			*n +=1;
		break;
		case '2':
			*n +=2;
		break;
		case '3':
			*n +=3;
		break;
		case '4':
			*n +=4;
		break;
		case '5':
			*n +=5;
		break;
		case '6':
			*n +=6;
		break;
		case '7':
			*n +=7;
		break;
		case '8':
			*n +=8;
		break;
		case '9':
			*n +=9;
		break;
		case 'a':
			*n +=10;
		break;
		case 'b':
			*n +=11;
		break;
		case 'c':
			*n +=12;
		break;
		case 'd':
			*n +=13;
		break;
		case 'e':
			*n +=14;
		break;
		case 'f':
			*n +=15;
		break;
		default:
			*n += 0;
	}
}
