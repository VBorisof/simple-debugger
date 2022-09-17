#!/bin/bash

gcc main.c -o debugger

./build-asm.sh hello-world.asm

./debugger hello-world

