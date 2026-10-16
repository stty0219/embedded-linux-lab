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
  sudo dmesg | tail -n 5
  sudo rmmod hello
```
* **Key Takeaways**:
  * Makefile uses `$(CURDIR)` instead of `$(PWD)`, so `M=` is not empty under `sudo`
  * Run `make` as a normal user; only `insmod` / `rmmod` need `sudo`
  * `Invalid module format` triage: `vermagic` vs `uname -r` -> `dmesg` -> `rpm -V kernel-devel`

### 02. Module Parameters (`02_module_param`)
A module that takes `name` and `count` at load time and exposes them under sysfs.

* **Source Files**: `hello_param.c`, `Makefile`
* **How to Build & Run**:
```bash
  make
  modinfo hello_param.ko
  sudo insmod hello_param.ko name=Stanley count=3
  sudo dmesg | tail -n 5
  cat /sys/module/hello_param/parameters/count
  sudo rmmod hello_param
```
* **Key Takeaways**:
  * `module_param(name, type, perm)` declares a parameter; `perm` sets its sysfs file mode
  * Changing a value via sysfs does not re-run `init`; it only updates the variable
  * `modinfo` shows each parameter and its `MODULE_PARM_DESC` text