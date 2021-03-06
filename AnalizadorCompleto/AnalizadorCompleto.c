#include<stdio.h>
#include <stdlib.h>
void analizarTrama(unsigned char *T);
void leerTrama(unsigned char *);
void analizaARP(unsigned char *);
void analizaIp(unsigned char *);
void decimal(char c, unsigned char *);
void ICMP(unsigned char *, int);
void TCP(unsigned char *, int);
void checksumTCP(unsigned char *, int, int);
void UDP(unsigned char *, int);
int main(){
	printf("Integrantes: \n");
	printf("Hernandez Vergara Eduardo\nRojas Cruz Jose Angel");
    FILE * ptr;
    char c;
	unsigned char hex = 0, ct = 0, p = 0, n = 0, i = 0;
	unsigned char nbytet[36];
	ptr = fopen("tramasIP.txt", "r");
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
	ptr = fopen("tramasIP.txt", "r");
		unsigned char *trama = (unsigned char*)(malloc(sizeof(char)*nbytet[0]));
		while(c != EOF){
			c = fgetc(ptr);
			if(c == '}'){
                leerTrama(trama);
                free(trama);
                i++;
                trama = (unsigned char*)(malloc(sizeof(char)*nbytet[i]));
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
            analizaIp(T);
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
    if(T[14]<<8 | (T[15] == 1)){
        printf("TIPO: ARP\n");
    }else if(T[14]<<8 | (T[15] == 6)){
        printf("IEEE 80.2 LAN\n");
    }else{
        printf("Otro: %d\n", (T[14]<<8 |T[15]));
    }
    // Tipo de direccion de Protocolo
    if (T[16]<<8 | (T[17] == 0x0806)){
        printf("TIPO: iPv4\n");
    }else{
        printf("TIPO: %.2x, %.2x\n", T[16], T[17]);
    }
    //Tama??o de la MAC
    printf("Tama??o MAC: %d bytes\n", T[18]);
    //Tama??o de la Direccion IP
    printf("Tama??o IP: %d bytes\n", T[19]);
    //Op Code
    if (T[20]<<8 | (T[21] == 1)){
        printf("Op Code: ARP Request\n");
    }else if (T[20]<<8 | (T[21] == 2)){
        printf("Op Code: ARP Reply\n");
    }else{
        printf("Otro: %d\n", (T[20]<<8 | T[21]));
    }
    printf("Direccion MAC origen: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", T[22], T[23], T[24], T[25], T[26], T[27]);
    printf("Direccion IP origen: %d.%d.%d.%d\n", T[28], T[29], T[30], T[31]);
    printf("Direccion MAC destino: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", T[32], T[33], T[34], T[35], T[36], T[37]);
    printf("Direccion IP destino: %d.%d.%d.%d\n", T[38], T[39], T[40], T[41]);
    
}
void analizaIp(unsigned char *T){
    int IHL = (T[14]&15) * 4;
	printf("Es de tipo IPv%d\n", T[14]>>4);
	if(T[15]&2){
		printf("Es un ts Costo minimo\n");
	}else if(T[15]&4){
		printf("Es un ts fiabilidad \n");
	}else if(T[15]&8){
		printf("Es un ts maximo rendimiento\n");
	}else if(T[15]&16){
		printf("Es un ts retardo minimo\n");
	}
    else{
        printf("Es un ts normal\n");
    }
    //Internet Header Length
    printf("Tama??o de cabecera: %d bytes\n", (T[14]&15) * 4);
    //Tipo de servicio
    printf("Tipo de servicio: %d\n", T[17]);
	printf("El tama??o total es de %d\n", T[16]<<8 | T[17]);
	printf("El id es de %d\n", T[18]<<8 | T[19]);
	if(T[20]&64){
		printf("Dont fragment\n");
	}else if(T[20]&32){
		printf("More Fragment\n");
	}
	printf("El desplazamiento del fragmento es de %d\n", (T[20]&31)<<8 | T[21]);
	printf("El tiempo de vida es %d\n", T[22]);
	if(T[23] == 1){
		printf("Es ICMP\n");
        ICMP(T, IHL);
	}else if(T[23] == 6){
        printf("Es TCP\n");
        TCP(T, IHL);
    }else if(T[23] == 17){
        printf("Es UDP\n");
        UDP(T, IHL);
    }else{
        printf("Es otro\n");
    }
    //Calcula y verifica ChecksumIP
    printf("--Checksum IP.--\n");
    unsigned char i = 0;
    unsigned short int sum = 0, chucksum = 0, aux;
    for(; i <= IHL +14-1; i+=2){
        sum += T[i]<<8 | T[i+1];
    }
    if((~((sum >> 16) + (sum & 0xFFFF)) & 0xFFFF) == 0x0000){
        printf("Checksum: Correcto\n");
        printf(":)\n");
    }else{
        printf("Checksum: Incorrecto\n");
        printf(":(\n");
        T[24] = 0x00;
        T[25] = 0x00;
        i = 0;
        sum = 0;
        for(; i <= T[14+IHL-1]; i+=2){
            sum += T[i]<<8 | T[i+1];
        }   
    }
    printf("Operacion Resultado: %.2x\n", sum);
    chucksum = (sum >> 16) + (sum & 0xFFFF);
    chucksum = 0xFFFF - chucksum;
    aux = chucksum;
    T[24] = aux/256;
    T[25] = aux%256;
    printf("Este es el chucksum correcto: %.2x\n", chucksum);
    printf(":)\n");
    chucksum = ~(chucksum + sum);
    if(chucksum == 0x0000){
        printf("Checksum: Correcto\n");
    }
    //datos
    printf("El checksum es: %2x\n", T[24]<<8 | T[25]);
	printf("Direccion IP origen: %d.%d.%d.%d\n", T[26], T[27], T[28], T[29]);
	printf("Direccion IP destino: %d.%d.%d.%d\n", T[30], T[31], T[32], T[33]);
    //opciones IHL
    printf("Opciones: ");
    if (((T[14]&15) * 4 > 20) && ((T[14]&15) * 4 < 60) ){
        int l;
        for (l = 34; l < ((T[14]&15)*4) + 14; l++){
            printf("%.2X ", T[l]);
        }
    }else{
        printf("No hay opciones\n");
    }
    
}
void ICMP(unsigned char *T, int IHL){
    printf("ICMP: ");
    printf("Tipo de respuesta ECO : %.2x\n", T[IHL+14]);
    if (T[IHL+14] == 0){
        printf("Tipo de respuesta: Echo Reply\n");
        //Datos echo reply
    }else if (T[IHL+14] == 8){
        printf("Tipo de respuesta: Echo Request\n");
    }
    printf("Mi codigo es: %.2x\n", T[IHL+15]);
    printf("Mi checksum es: %.2x\n", T[IHL+16] << 8 | T[IHL+17]);
    printf("El id es: %.2x\n", T[IHL+18] << 8 | T[IHL+19]);
    printf("El numero de secuencia es: %.2x\n", T[IHL+20] << 8 | T[IHL+21]);
}
void TCP(unsigned char *T, int IHL){
    printf("TCP: ");
    printf("Puerto origen: %d\n", T[IHL+14]<<8 | T[IHL+15]);
    printf("Puerto destino: %d\n", T[IHL+16]<<8 | T[IHL+17]);
    printf("Numero de secuencia: %d\n", T[IHL+18]<<24 | T[IHL+19]<<16 | T[IHL+20]<<8 | T[IHL+21]);
    printf("Numero de ack: %d\n", T[IHL+22]<<24 | T[IHL+23]<<16 | T[IHL+24]<<8 | T[IHL+25]);
    printf("Tama??o de la cabecera: %d\n", (T[IHL+26]>>4) * 4);
    int offset = (T[IHL+26]&15) * 4;
    //Banderas
    printf("Banderas: ");
    if(T[IHL+27]&1){
        printf("FIN\n");
    }
    if(T[IHL+27]&2){
        printf("SYN\n");
    }
    if(T[IHL+27]&4){
        printf("RST\n");
    }
    if(T[IHL+27]&8){
        printf("PSH\n");
    }
    if(T[IHL+27]&16){
        printf("ACK\n");
    }
    if(T[IHL+27]&32){
        printf("URG\n");
    }
    if(T[IHL+27]&64){
        printf("ECE\n");
    }
    if(T[IHL+27]&128){
        printf("CWR\n");
    }
    //Ventana
    printf("Ventana: %d\n", T[IHL+28]<<8 | T[IHL+29]);
    //Checksum
    checksumTCP(T, offset, IHL);
    //Puntero Urgente
    printf("Puntero Urgente: %d\n", T[IHL+32]<<8 | T[IHL+33]);
    //Opciones
    printf("Opciones: ");
    if ((T[IHL+26]&15) * 4 > 20 && (T[IHL+26]&15) * 4 < 60 ){
        int l;
        for (l = 34; l < (T[IHL+26]&15) + 34; l++){
            printf("%.2X ", T[l]);
        }
    }else{
        printf("No hay opciones\n");
    }
}
void checksumTCP(unsigned char *T, int offset, int IHL){
    unsigned char i = 0;
    unsigned short int checksum = 0, aux = 0;
    int sum = 0;
    for(; i <= IHL+14+offset; i+=2){
        sum += T[IHL+14+i]<<8 | T[IHL+14+i+1];
    }
    sum += T[26]<<8 | T[27];
    sum += T[28]<<8 | T[29];
    sum += T[30]<<8 | T[31];
    sum += T[32]<<8 | T[33];
    sum += T[13]<<8 | T[23];
    sum += 0x00| ((T[IHL+26]<<4)*4)>>8 ;
    //printf("Operacion Resultado: %x\n", sum);
    aux = (sum >> 16) + (sum & 0xFFFF);
    aux = 0xFFFF - aux;
    checksum = (T[IHL+30] << 8 | T[IHL+31]);
    printf("Checksum: %.2x\n", checksum);
    
    if(aux == 0){
        printf("Checksum: Correcto\n");
    }else{
        printf("Checksum: Incorrecto\n");
        T[IHL+30] = 0x00;
        T[IHL+31] = 0x00;
        i = 0;
        sum = 0;
        for(; i <= IHL+14+offset; i+=2){
            sum += T[i]<<8 | T[i+1];
        }   
        sum += T[26]<<8 | T[27];
        sum += T[28]<<8 | T[29];
        sum += T[30]<<8 | T[31];
        sum += T[32]<<8 | T[33];
        sum += T[13]<<8 | T[23];
        sum += T[IHL+26]<<8 | 0x00;

        //printf("Operacion Resultado: %.2x\n", sum);
        checksum = (~((sum >> 16) + (sum & 0xFFFF)) & 0xFFFF);
        if(checksum == 0x0000){
            printf("Checksum: Correcto\n");
        }
    }
}
void UDP(unsigned char *T, int IHL){
    printf("UDP: \n");
    printf("Puerto origen: %d\n", T[IHL+14]<<8 | T[IHL+15]);
    printf("Puerto destino: %d\n", T[IHL+16]<<8 | T[IHL+17]);
    printf("Tama??o de la cabecera: %d\n", (T[IHL+18]>>4) * 4);
    printf("Checksum: %.2x %.2x\n", T[IHL+19]<<8,  T[IHL+20]);
    //Opciones
    printf("Opciones: ");
    if ((T[IHL+18]&15) * 4 > 20 && (T[IHL+18]&15) * 4 < 60 ){
        int l;
        for (l = 20; l < (T[IHL+18]&15) + 20; l++){
            printf("%.2X ", T[l]);
        }
        printf("\n");
    }else{
        printf("No hay opciones\n");
    }
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
        case 'A':
			*n +=10;
		break;
		case 'b':
        case 'B':
			*n +=11;
		break;
		case 'c':
        case 'C':
			*n +=12;
		break;
		case 'd':
        case 'D':
			*n +=13;
		break;
		case 'e':
        case 'E':
			*n +=14;
		break;
		case 'f':
        case 'F':
			*n +=15;
		break;
		default:
			*n += 0;
	}
}
