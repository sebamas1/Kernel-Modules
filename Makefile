CONFIG_MODULE_SIG=n

CFLAGS_encriptador.o := -Wall -Werror -Wextra -Wconversion -std=gnu11 #-pedantic
CFLAGS_desencriptador.o := -Wall -Werror -Wextra -Wconversion -std=gnu11 #-pedantic

obj-m += encriptador.o desencriptador.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	
	