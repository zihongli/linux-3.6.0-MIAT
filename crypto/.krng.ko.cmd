cmd_crypto/krng.ko := arm-xilinx-linux-gnueabi-ld -EL -r  -T /home/z/zedboard/MIAT/linux-3.6.0-MIAT/scripts/module-common.lds --build-id  -o crypto/krng.ko crypto/krng.o crypto/krng.mod.o
