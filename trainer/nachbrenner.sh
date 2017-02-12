#!/bin/bash

# !!!!!!!!!!!!!!!!!!
#chmod +x nachbrenner.sh
#./nachbrenner.sh
# !!!!!!!!!!!!!!!!!!

cd ~/dev/udoo-community-bsp
MACHINE=udooneo DISTRO=fslc-framebuffer source ./setup-environment build
sudo pip3 install -r ~/dev/udoo-community-bsp/sources/poky/bitbake/toaster-requirements.txt

# Note: As far s I remember this once contained also
# rm -rf ~/dev/udoo-community-bsp/build/tmp (or something like this)

cd ~/dev/udoo-community-bsp
MACHINE=udooneo DISTRO=fslc-framebuffer source ./setup-environment build
MACHINE=udooneo bitbake udoo-image-full-cmdline
