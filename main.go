package main

/*
#cgo CFLAGS: -Iinclude
#cgo LDFLAGS: -L${SRCDIR}/build -lbulkifx_static
#include "include/bulkifx/grayscale/grayscale.h"
#include "include/bulkifx/sepia/sepia.h"
#include "include/bulkifx/padding/zero_padding.h"
*/
import "C"

import (
	"fmt"
	"os"
)

//export Grayscaling
func Grayscaling(pathIn *C.char, pathOut *C.char) C.int {
	err := ImagePipeline(C.GoString(pathIn), C.GoString(pathOut), nil, func(inPtr, outPtr *C.uint8_t, w, h int) {
		C.c_grayscale(inPtr, outPtr, C.int(w), C.int(h))
	})
	if err != nil {
		fmt.Fprintf(os.Stderr, "bulkifx: %v\n", err)
		return 1
	}
	return 0
}

//export Sepia
func Sepia(pathIn *C.char, pathOut *C.char) C.int {
	err := ImagePipeline(C.GoString(pathIn), C.GoString(pathOut), nil, func(inPtr, outPtr *C.uint8_t, w, h int) {
		C.c_sepia(inPtr, outPtr, C.int(w), C.int(h))
	})
	if err != nil {
		fmt.Fprintf(os.Stderr, "bulkifx: %v\n", err)
		return 1
	}
	return 0

}

//export ZeroPadding
func ZeroPadding(pathIn *C.char, pathOut *C.char, padding C.short) C.int {
	err := ImagePipeline(C.GoString(pathIn), C.GoString(pathOut), func(w, h int) (int, int) {
		return w + int(padding)*2, h + int(padding)*2
	}, func(inPtr, outPtr *C.uint8_t, w, h int) {
		C.c_zero_padding(inPtr, outPtr, C.int(w), C.int(h), padding)
	})
	if err != nil {
		fmt.Fprintf(os.Stderr, "bulkifx: %v\n", err)
		return 1
	}
	return 0

}

func main() {}
