# 欢迎使用 OwO-kernel
## 简介

待后续发布纯手写的 **OwO-kernel-2**。
> 呃很垃圾就对了。

## 功能特点

- **何意味？**: 可以在VGA文本模式下现实字符
- **未完待续**: 以后还会更新

## 实现的功能
启动后会输出Hello, OwO-kernel World!
没了...
## 快速开始

**如何编译 OwO-kernel ？**
1. 你需要准备一个GCC交叉编译器，然后克隆仓库到本地。
2. 编译引导程序boot.s。
```bash
i686-elf-as boot.s -o boot.o
```
3. 编译内核kernel.c。
```bash
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
```
4. 构建最终的内核。
```bash
i686-elf-gcc -T linker.ld -o owo -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
```
5. 构建可引导的cdrom映像。
```bash
cp owo isodir/boot/owo
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o owo.iso isodir
```
6.这样就已经构建了owo.iso。

**如何在虚拟机运行操作系统？**
1. 自行安装QEMU。
2. 启动系统。
```bash
qemu-system-i386 -cdrom owo.iso
```

不知道写什么了。