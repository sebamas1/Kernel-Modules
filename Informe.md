# Kernel modules


### Método de uso

Primero se debe compilar el programa, para lo cual, se debe ir al directorio del projecto y ejecutar el comando  _make_
Una vez hecho esto, se han compilado los módulos, ahora es necesario cargarlos, y asociarles un device file. 

Para esto, primero se deben cargar los módulos:
 
     make load 
 Luego se deben crear y asociar los device file correspondientes. Los comandos necesarios para esto, se pueden encontrar ejecutando *dmesg*, ya que al cargar los módulos, estos dejan un mensaje con el comando preparado. Se recomienda redireccionar el output hacia tail para mayor facilidad, de la siguiente forma:
 
    dmesg | tail
    
Obteniendo el siguiente output:

``` 
.
.
.
[ 3307.974435] encriptador: major number is 240 
[ 3307.974438] 	use "sudo mknod /dev/encriptador c 240 0" for device file
[ 3315.045055] desencriptador: major number is 239 
[ 3315.045057] 	use "sudo mknod /dev/desencriptador c 239 0" for device file


```

Ejecutando los comandos que nos indica, se crearan los device file necesarios, como se puede ver si se ejecuta el comando ls /dev.
Ahora es necesario permitir que el userapp creada pueda escribir sobre ese device file, por lo que se necesitan ejecutar los siguientes comandos:

    sudo chmod 777 /dev/encriptador
    sudo chmod 777 /dev/desencriptador
    
Hecho esto, ya se puede ejecutar la userapp con el comando 

    ./userapp
    
Ahora, la userapp pide que se le indique si se quiere operar sobre el encriptador, o el desencriptador. Ingresar  __e__  si se quiere operar sobre el encriptador, o  __d__  para el desencriptador.

```
./userapp                         
Ingresar d para escribir/leer sobre desencriptador o e para escribir/leer sobre encriptador:

```

En caso de querer usar el encriptador, ahora se debe ingresar  __r__  en caso de querer leer la información guardada, o  __w__  si se quiere escribir nueva información a encriptar.

```
./userapp                         
Ingresar d para escribir/leer sobre desencriptador o e para escribir/leer sobre encriptador: e
Ingresar r para leer o w para escribir: 
```

Si se usa el desencriptador, la userapp directamente escribe sobre este, y luego lee la informacion desencriptada imprimiendola en el stdout.

Si se intenta ingresar una cadena superior a 1000 caracteres(que es el tamaño del buffer de los módulos), no se permite escribir nada y se imprime un mensaje en dmesg indicando un buffer overflow.

Es posible tambien usar echo directamente para escribir sobre los device file, pero se han encontrado mas errores relacionados a un buffer overflow que sospecho que tiene que ver con el funcionamiento interno de echo. Parece que echo siempre intenta escribir más caracteres de lo esperado.

Nota: siempre ejecutar el comando echo como root.

### Desarrollo

El código esta fuertemente basado en el código del canal de youtube "SolidusCode". Se define una estructura file_operations que contiene las funciones read, write, open y close que definen el comportamiento que se debe tomar cuando se intenta operar sobre el device file asociado al módulo. Además existe un sistema de semáforos que impiden que varios procesos usen los módulos al mismo tiempo.

Las funciones principalmente modificadas fueron la de read y write. Se modificó la función de write del encriptador para que sume un entero positivo a cada caracter, "encriptando" de esta forma, la cadena. La funcion de write del desencriptador fue modificada para que reste el mismo entero a la cadena, "desencriptando" la información.

#### userapp

La userapp se encarga de facilitar la demostración del funcionamiento de los módulos. Lo que hace esta, es operar sobre los device file que estan asociados a los módulos. Cuando se quiere escribir sobre el encriptador, llama a la función write de este, y cuando se quiere leer la información encriptada, llama a la función read. Si se desea usar el desencriptador, la userapp lee la información encriptada guardada en el módulo encriptador, la escribe en el device file asociado al desencriptador, y luego lee la información desencriptada del módulo desencriptador para luego imprimirla por la stdout.

#### Bugs conocidos

Si primero se ingresa una cadena larga, y luego se ingresa una cadena más corta(al encriptador), puede quedar un residuo de la anterior cadena. Sin embargo, considero que no era necesario arreglarlo, ya que no es el propósito del TP.

No me fue posible agregar la flag pedantic al makefile porque reportaba una warning(tratada como error) asociada al makefile. Este makefile esta basado en el README y no pude encontrar en ningun lugar como arreglar estas warning.


