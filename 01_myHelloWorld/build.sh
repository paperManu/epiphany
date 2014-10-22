#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/legacy.ldf

SCRIPT=$(readlink -f "$0")
EXEPATH=$(dirname "$SCRIPT")

mkdir -p build

echo "Building host..."
g++ -std=c++11 -Os src/main.cpp -o build/main.elf -I ${EINCS} -L ${ELIBS} -le-hal -lpthread
echo "Building epiphany..."
e-g++ -Os -T ${ELDF} src/e_main.cpp -o build/e_main.elf -le-lib
echo "Convert epiphany elf to srec..."
e-objcopy --srec-forceS3 --output-target srec build/e_main.elf build/e_main.srec
