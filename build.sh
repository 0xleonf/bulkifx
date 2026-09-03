#!/bin/bash
set -e
cd "$(dirname "$0")"

mkdir -p build
cmake -S . -B build
cmake --build build

go build -buildmode=c-shared -o build/bulkifx.so .
