cmd_arch/arm/lib/csumipv6.o := arm-xilinx-linux-gnueabi-gcc -Wp,-MD,arch/arm/lib/.csumipv6.o.d  -nostdinc -isystem /home/z/Xilinx/SDK/2014.2/gnu/arm/lin/bin/../lib/gcc/arm-xilinx-linux-gnueabi/4.8.1/include -I/home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include /home/z/zedboard/MIAT/linux-3.6.0-MIAT/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-zynq/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables  -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float        -c -o arch/arm/lib/csumipv6.o arch/arm/lib/csumipv6.S

source_arch/arm/lib/csumipv6.o := arch/arm/lib/csumipv6.S

deps_arch/arm/lib/csumipv6.o := \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/linkage.h \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/assembler.h \
    $(wildcard include/config/cpu/feroceon.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/cpu/use/domains.h) \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/hwcap.h \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \

arch/arm/lib/csumipv6.o: $(deps_arch/arm/lib/csumipv6.o)

$(deps_arch/arm/lib/csumipv6.o):
