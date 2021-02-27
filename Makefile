##CFLAGS_encriptador.o := -Wall -Werror -Wextra -Wconversion -std=gnu11 #-pedantic

CONFIG_MODULE_SIG=n

obj-m += encriptador.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	
	