cmd_kernel/sched/built-in.o :=  arm-xilinx-linux-gnueabi-ld -EL    -r -o kernel/sched/built-in.o kernel/sched/core.o kernel/sched/clock.o kernel/sched/idle_task.o kernel/sched/fair.o kernel/sched/rt.o kernel/sched/stop_task.o kernel/sched/cpupri.o 
