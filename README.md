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

### 03. Character Device Driver (`03_char_driver`)
A minimal character driver exposing `/dev/mychar`: data written from user space is stored in a kernel buffer and can be read back.

* **Source Files**: `mychar.c`, `Makefile`
* **How to Build & Run**:
```bash
  make
  sudo insmod mychar.ko
  echo "hello kernel" | sudo tee /dev/mychar
  sudo cat /dev/mychar
  sudo dmesg | tail -n 8
  sudo rmmod mychar
```
* **Key Takeaways**:
  * `alloc_chrdev_region` + `cdev_add` bind a major:minor to a `file_operations` table
  * `class_create` + `device_create` let udev create `/dev/mychar` automatically
  * `copy_to_user` / `copy_from_user` return the number of bytes *not* copied; non-zero means `-EFAULT`
  * `read` must honor `*ppos` and return 0 at the end, otherwise `cat` never terminates
  * Error paths in `init` unwind with `goto` labels in reverse order
  * Known limitation: global buffer without locking (race condition), to be addressed in a later experiment