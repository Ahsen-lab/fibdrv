#!/usr/bin/env bash

# Back up the original CPU-related settings.
CPUMASK=3
ASLR=$(cat /proc/sys/kernel/randomize_va_space)
CPU_FREQ=$(cat /sys/devices/system/cpu/cpu${CPUMASK}/cpufreq/scaling_governor)
TURBO=$(cat /sys/devices/system/cpu/intel_pstate/no_turbo)
SMT=$(cat /sys/devices/system/cpu/smt/control)

make unload
make clean
make
make load


sudo bash -c "echo 0 > /proc/sys/kernel/randomize_va_space"
sudo bash -c "echo performance > /sys/devices/system/cpu/cpu${CPUMASK}/cpufreq/scaling_governor"
sudo bash -c "echo 1 > /sys/devices/system/cpu/intel_pstate/no_turbo"
sudo bash -c "echo off > /sys/devices/system/cpu/smt/control"


sudo taskset -c ${CPUMASK} ./client_bn
gnuplot bn_fib.gp


sudo bash -c "echo ${ASLR} > /proc/sys/kernel/randomize_va_space"
sudo bash -c "echo ${CPU_FREQ} > /sys/devices/system/cpu/cpu${CPUMASK}/cpufreq/scaling_governor"
sudo bash -c "echo ${TURBO} > /sys/devices/system/cpu/intel_pstate/no_turbo"
sudo bash -c "echo ${SMT} > /sys/devices/system/cpu/smt/control"

