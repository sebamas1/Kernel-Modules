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

#define ENCRIPTADOR "/dev/encriptador"
#define DESENCRIPTADOR "/dev/desencriptador"

void operar_encriptador(){
	int fd_encriptador;
	char read_write_choice, read_buffer[1000], write_buffer[1000];

	fd_encriptador = open(ENCRIPTADOR, O_RDWR);

	if (fd_encriptador == -1) {
		fprintf(stderr,
				"El device file no existe o los modulos no estan cargados.\n");
		exit(EXIT_FAILURE);
	}

	printf("Ingresar r para leer o w para escribir: ");

	scanf(" %c", &read_write_choice);

	switch (read_write_choice) {
	case 'r':
		read(fd_encriptador, read_buffer, sizeof(read_buffer));
		printf("TP4: %s\n", read_buffer);
		break;
	case 'w':
		printf("Ingresar la cadena a encriptar: ");
		scanf(" %[^\n]", write_buffer);
		write(fd_encriptador, write_buffer, sizeof(write_buffer));
		break;
	default:
		printf("encriptador: Comando no soportado.\n");
		break;
	}
	close(fd_encriptador);
}

void operar_desencriptador(){
	int fd_desencriptador, fd_encriptador;
		char read_write_choice, encrypted_buffer[1000], decrypted_buffer[1000];

		fd_encriptador = open(ENCRIPTADOR, O_RDWR);
		fd_desencriptador = open(DESENCRIPTADOR, O_RDWR);

		if (fd_desencriptador == -1 || fd_encriptador == -1) {
			fprintf(stderr,
					"El device file no existe o los modulos no estan cargados.\n");
			exit(EXIT_FAILURE);
		}

			read(fd_encriptador, encrypted_buffer, sizeof(encrypted_buffer));
			write(fd_desencriptador, encrypted_buffer, sizeof(encrypted_buffer));
			read(fd_desencriptador, decrypted_buffer, sizeof(decrypted_buffer));
			printf("TP4: %s\n", decrypted_buffer);

		close(fd_desencriptador);
		close(fd_encriptador);
}


int main() {;
	char ch;

	printf(
			"Ingresar d para escribir/leer sobre desencriptador o e para escribir/leer sobre encriptador: ");
	scanf("%c", &ch);

	switch (ch) {
	case 'd':
		operar_desencriptador();
		break;
	case 'e':
		operar_encriptador();
		break;
	default:
		printf("Comando no soportado.\n");
		break;
	}
	return 0;
}

