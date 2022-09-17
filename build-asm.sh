#!/bin/bash

progname=$(echo "$1" | cut -f 1 -d '.')

echo "Assembling $progname.o"
nasm -f elf64 "$1" -o "$progname.o"

echo "Linking $progname"
ld "$progname.o" -o "$progname"

echo "[+] Done."
