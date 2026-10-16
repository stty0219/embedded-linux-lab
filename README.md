# Embedded Linux Lab

A collection of my hands-on experiments for Linux Kernel module development and embedded systems.

## Experiments

### 01. Hello World Kernel Module (`01_hello_kernel_module`)
A basic loadable kernel module that demonstrates the core lifecycle of a Linux kernel module.

* **Source Files**: `hello.c`, `Makefile`
* **How to Build & Run**:
  ```bash
  make
  sudo insmod hello.ko
  dmesg | tail -n 5
  sudo rmmod hello