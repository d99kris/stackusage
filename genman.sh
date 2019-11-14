#!/bin/bash

cd src && \
help2man -n "measure stack usage in applications" -N -o stackusage.1 ./stackusage && \
cd .. && \
mkdir -p build && cd build && cmake .. && make -s


