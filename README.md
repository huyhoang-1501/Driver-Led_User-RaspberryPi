<div align="center">
  <h1>Raspberry Pi 4 GPIO Control Driver & User Interface</h1>
  <p>
    <img src="https://img.shields.io/badge/Platform-Raspberry%20Pi%204B-red?style=for-the-badge&logo=raspberry-pi&logoColor=white" alt="Raspberry Pi 4B">
    <img src="https://img.shields.io/badge/Kernel-Linux%205.15%2B-blue?style=for-the-badge&logo=linux&logoColor=white" alt="Linux Kernel">
    <img src="https://img.shields.io/badge/Driver-Character%20Device-green?style=for-the-badge&logo=c&logoColor=white" alt="Char Driver">
    <img src="https://img.shields.io/badge/GPIO-BCM2711-orange?style=for-the-badge&logo=raspberry-pi&logoColor=white" alt="BCM2711 GPIO">
    <img src="https://img.shields.io/badge/Language-C%20&%20Makefile-critical?style=for-the-badge&logo=c&logoColor=white" alt="C Language">
    <img src="https://img.shields.io/badge/Architecture-ARM64-blueviolet?style=for-the-badge&logo=arm&logoColor=white" alt="ARM64">
    <img src="https://img.shields.io/badge/Toolchain-aarch64--linux--gnu--gnu-brightgreen?style=for-the-badge&logo=gnu&logoColor=white" alt="Cross Compile">
  </p>
  ---
  <p>
    <a href="#-overview">Overview</a> •
    <a href="#-key-features">Key Features</a> •
    <a href="#-setup-guide">Setup Guide</a> •
    <a href="#-usage">Usage</a> •
    <a href="#-testing">Testing</a>
  </p>
  ---
</div>
<br>

## Overview
**`RPi4 GPIO Control Driver`** is a **custom Linux character device driver** for **Raspberry Pi 4 Model B**.  
It allows **userspace applications** to toggle a GPIO pin by writing simple strings:

- `"bat"` → **set** (turn ON)  
- `"tat"` → **clear** (turn OFF)

The project includes:

* **Kernel module** (`driver_rpi4.ko`) with direct memory-mapped access to BCM2711 GPIO registers  
* **Interactive CLI tool** (`user_control`) – press `0` / `1` / `q`  
* **Full cross-compilation environment** for ARM64  
* **Custom kernel build & SD-card deployment pipeline**

> **Ideal for**: Linux driver development, embedded systems labs, IoT prototyping, GPIO automation.

---

## Key Features
| Feature | Description |
|--------|-------------|
| **Direct GPIO Mapping** | `ioremap(0xFE200000, PAGE_SIZE)` for BCM2711 registers |
| **Command Interface** | Write `"bat"` → GPIO high, `"tat"` → GPIO low |
| **Character Device** | `/dev/driver` with `read`/`write` |
| **CLI Control** | `0` = ON, `1` = OFF, `q` = quit |
| **Cross-Compiled** | Uses `aarch64-linux-gnu-` toolchain |
| **Auto Device Node** | `class_create()` + `device_create()` |
| **Safe Memory Ops** | `copy_to_user`, `copy_from_user`, `iounmap` |
| **Robust Init/Exit** | Full cleanup on failure |

---

## Setup Guide

### 1. Host PC (Ubuntu) – Install Tools

* sudo apt update
* sudo apt install -y git bc bison flex libssl-dev make libc6-dev libncurses5-dev
* sudo apt install -y crossbuild-essential-arm64

### 2. Clone & Configure Raspberry Pi Kernel

* mkdir -p ~/kernel_custom && cd ~/kernel_custom
* git clone --depth=1 -b rpi-5.15.y https://github.com/raspberrypi/linux
* cd linux
* KERNEL=kernel8
* make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- bcm2711_defconfig
###  (Optional) make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- menuconfig

### 3. Build Kernel, Modules & DTBs

* make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- Image modules dtbs -j$(nproc)

### 4. Build the Driver Module

* Place driver_rpi4.c and its Makefile in a folder (e.g., ~/rpi4-driver/):
* Makefile (driver)
* cd ~/rpi4-driver
* make
#### → driver_rpi4.ko

### 5. Deploy Kernel to SD Card

### Insert RPi SD card → identify with lsblk (e.g., /dev/sdb)
* sudo mkdir -p mnt/boot mnt/root
* sudo mount /dev/sdb1 mnt/boot
* sudo mount /dev/sdb2 mnt/root

### Install modules
* sudo env PATH=$PATH make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- \
     * * INSTALL_MOD_PATH=$(pwd)/mnt/root modules_install

### Backup & copy kernel
* sudo cp mnt/boot/$KERNEL.img mnt/boot/$KERNEL-backup.img
* sudo cp arch/arm64/boot/Image mnt/boot/$KERNEL.img
* sudo cp arch/arm64/boot/dts/broadcom/*.dtb mnt/boot/
* sudo cp arch/arm64/boot/dts/overlays/*.dtb* mnt/boot/overlays/
* sudo cp arch/arm64/boot/dts/overlays/README mnt/boot/overlays/

### Edit config.txt
* sudo nano mnt/boot/config.txt
### Add or ensure:
**[all]**
* kernel=kernel8.img
* sudo umount mnt/boot mnt/root

### 6. Load Driver on RPi

* scp driver_rpi4.ko pi@<rpi-ip>:/home/pi/
* ssh pi@<rpi-ip>

* sudo insmod /home/pi/driver_rpi4.ko
* dmesg | tail -n 20
* ls -l /dev/driver

### 7. Build & Run Userspace App

* user_control.c (already provided)
* Makefile (userspace)

* make
* scp user_control pi@<rpi-ip>:/home/pi/
* ssh pi@<rpi-ip>
* sudo ./user_control

### Usage Direct Shell Commands

* echo "bat" | sudo tee /dev/driver   # Turn ON
* echo "tat" | sudo tee /dev/driver   # Turn OFF

#### Read Test

* cat /dev/driver
### → Hello user

### Kernel Logs

* dmesg | grep -i driver

## Image
### Copy Led User To Raspberry Pi 4:
![Image](https://github.com/user-attachments/assets/d5254b2a-7862-47db-b946-c4bba494f83f)

### Implement on Ubuntu: 
![Image](https://github.com/user-attachments/assets/8a5fde29-6cf1-4977-b8b2-febd4a9ce15c)

<div align="center">
  <p><strong>© 2025 – Embedded Linux Driver Project</strong></p>
  <p><i>Nguyễn Phạm Huy Hoàng - 22161125</i> 
  <p><em>Low-level control. High-level simplicity. GPIO mastery.</em></p>
</div>