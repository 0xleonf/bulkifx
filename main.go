package main

/*
#cgo CFLAGS: -Iinclude
#cgo LDFLAGS: -L${SRCDIR}/build -lbulkifx_static
#include "include/bulkifx/grayscale/grayscale.h"
*/
import "C"

import (
	"fmt"
	"os"
)

//export Grayscaling
func Grayscaling(pathIn *C.char, pathOut *C.char) C.int {
	err := ImagePipeline(C.GoString(pathIn), C.GoString(pathOut), func(inPtr, outPtr *C.uint8_t, w, h int) {
		C.c_grayscale(inPtr, outPtr, C.int(w), C.int(h))
	})
	if err != nil {
		fmt.Fprintf(os.Stderr, "bulkifx: %v\n", err)
		return 1
	}
	return 0
}

func main() {}
