#!/bin/sh -eu

if [ "${1:-}" = "clean" ]
then
  make ${MAKE_OPTS:-} -C kernel clean
  exit 0
fi

make ${MAKE_OPTS:-} -C kernel kernel.elf

OS_DIR=$PWD

if [ "${1:-}" = "run" ]
then
  cd ~/edk2
  ~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi ~/edk2/Build/RunPkgX64/DEBUG_CLANG38/X64/Loader.efi $OS_DIR/kernel/kernel.elf
  cd $OSDIR
fi