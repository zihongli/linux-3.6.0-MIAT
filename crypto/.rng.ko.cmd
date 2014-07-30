cmd_crypto/rng.ko := arm-xilinx-linux-gnueabi-ld -EL -r  -T /home/z/zedboard/MIAT/linux-3.6.0-MIAT/scripts/module-common.lds --build-id  -o crypto/rng.ko crypto/rng.o crypto/rng.mod.o
