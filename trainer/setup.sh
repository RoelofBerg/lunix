#!/bin/bash

# !!!!!!!!!!!!!!!!!!
#chmod +x setup.sh
#./setup.sh
# !!!!!!!!!!!!!!!!!!

sudo apt-get update
sudo apt-get install git git-gui putty nano scite texinfo chrpath gawk wget git \
     diffstat unzip texinfo gcc-multilib build-essential chrpath socat libsdl1.2-dev \
     xterm picocom ncurses-dev lzop libncurses-dev python3-pip pbzip2
# The following must happen afterwards
sudo apt-get install gcc-arm-linux-gnueabihf

mkdir ~/transfer
cd ~/transfer

# !!!!! NAME !!!!!
cp /media/s2/UBUNTU\ 16_0/TRANSFER/* ~/transfer
# !!!!!!!!!!!!!!!!

cat lunixa* > lunix.tgz
rm -rf lunixa*
cd ~
tar -xf ~/transfer/lunix.tgz --use-compress-prog=pbzip2

sync
echo .
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo YOU CAN REMOVE THE USB STICK NOW and press return
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

read test123

git config --global user.email "lunix@draeger.com"
git config --global user.name "Lunix Luna"

cd ~/dev/udoo-community-bsp
MACHINE=udooneo DISTRO=fslc-framebuffer source ./setup-environment build
sudo pip3 install -r ~/dev/udoo-community-bsp/sources/poky/bitbake/toaster-requirements.txt

cd ~/dev/linux_kernel
ARCH=arm make udoo_neo_defconfig
#ARCH=arm make menuconfig
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make zImage -j4
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make dtbs -j4
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make modules -j4

cd ~/dev/udoo-community-bsp
MACHINE=udooneo DISTRO=fslc-framebuffer source ./setup-environment build
MACHINE=udooneo bitbake udoo-image-full-cmdline




