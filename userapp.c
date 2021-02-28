/*
 * userapp.c
 *
 *  Created on: 28 feb. 2021
 *      Author: sebastian
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/TP4"

int main(){
	int i, fd;
	char ch, read_buffer[1000], write_buffer[1000];
	fd = open(DEVICE, O_RDWR); //abre para leer y escribir
	if(fd == -1){
		fprintf(stderr, "El device file no existe o los modulos no estan cargados.\n");
		exit(EXIT_FAILURE);
	}

	printf("Ingresar r para lectura o w para escritura: ");
	scanf("%c", &ch);
	switch(ch){
		case 'r':
			read(fd, read_buffer, sizeof(read_buffer));
			printf("TP4: %s\n", read_buffer);
			break;
		case 'w':
			printf("Ingresar la cadena a encriptar: ");
			scanf(" %[^\n]", write_buffer);
			write(fd, write_buffer, sizeof(write_buffer));
			break;
		default:
			printf("Comando no soportado.\n");
			break;
	}
	close(fd);
	return 0;
}

