#!/bin/bash

mkdir -p build && cd build && cmake .. && make -s && \
help2man -n "measure stack usage in applications" -N -o stackusage.1 ./stackusage
