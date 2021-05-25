#!/bin/sh

make clean
make
cp solo_app.bin /tftpboot/
