cmd_arch/arm/kernel/entry-common.o := arm-xilinx-linux-gnueabi-gcc -Wp,-MD,arch/arm/kernel/.entry-common.o.d  -nostdinc -isystem /home/z/Xilinx/SDK/2014.2/gnu/arm/lin/bin/../lib/gcc/arm-xilinx-linux-gnueabi/4.8.1/include -I/home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include /home/z/zedboard/MIAT/linux-3.6.0-MIAT/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-zynq/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables  -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float        -c -o arch/arm/kernel/entry-common.o arch/arm/kernel/entry-common.S

source_arch/arm/kernel/entry-common.o := arch/arm/kernel/entry-common.S

deps_arch/arm/kernel/entry-common.o := \
    $(wildcard include/config/need/ret/to/user.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/function/tracer.h) \
    $(wildcard include/config/old/mcount.h) \
    $(wildcard include/config/frame/pointer.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/dynamic/ftrace.h) \
    $(wildcard include/config/oabi/compat.h) \
    $(wildcard include/config/arm/thumb.h) \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/aeabi.h) \
    $(wildcard include/config/alignment/trap.h) \
    $(wildcard include/config/seccomp.h) \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/unistd.h \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/ftrace.h \
    $(wildcard include/config/arm/unwind.h) \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/unwind.h \
  arch/arm/kernel/entry-header.S \
    $(wildcard include/config/cpu/v6.h) \
    $(wildcard include/config/cpu/32v6k.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  arch/arm/include/generated/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/linkage.h \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/linkage.h \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/assembler.h \
    $(wildcard include/config/cpu/feroceon.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/cpu/use/domains.h) \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/ptrace.h \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/hwcap.h \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  arch/arm/include/generated/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /home/z/zedboard/MIAT/linux-3.6.0-MIAT/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  arch/arm/kernel/calls.S \

arch/arm/kernel/entry-common.o: $(deps_arch/arm/kernel/entry-common.o)

$(deps_arch/arm/kernel/entry-common.o):
