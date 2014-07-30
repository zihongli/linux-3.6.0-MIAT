cmd_drivers/gpio/built-in.o :=  arm-xilinx-linux-gnueabi-ld -EL    -r -o drivers/gpio/built-in.o drivers/gpio/gpiolib.o drivers/gpio/devres.o drivers/gpio/gpiolib-of.o drivers/gpio/gpio-xilinxps.o 
