CONFIG_MODULE_SIG=n

CFLAGS_encriptador.o := -Wall -Werror -Wextra -Wconversion -std=gnu11 #-pedantic
CFLAGS_desencriptador.o := -Wall -Werror -Wextra -Wconversion -std=gnu11 #-pedantic
CFLAGS = -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

obj-m += encriptador.o desencriptador.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc $(CFLAGS) -o userapp userapp.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f *.o
	rm -f userapp
	
	