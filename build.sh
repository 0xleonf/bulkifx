#!/bin/bash
set -e
cd "$(dirname "$0")"

mkdir -p build
cmake -S . -B build
cmake --build build

ctest --test-dir build --output-on-failure

go build -buildmode=c-shared -o build/bulkifx.so .
