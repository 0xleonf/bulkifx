package main

/*
#include <stdint.h>
#include "include/bulkifx/grayscale/grayscale.h"
*/
import "C"

import (
	"fmt"
	"image"
	"image/draw"
	_ "image/jpeg"
	"image/png"
	"os"
	"unsafe"
)

type ImageOperation func(inputPtr, outputPtr *C.uint8_t, width, height int)

// PNG deflate dominates pipeline time (~150ms of ~200ms on 1.2MP).
// BestSpeed cuts ~35ms with only ~8% larger files. Switch to
// png.DefaultCompression if smallest output matters more than speed.
var pngEncoder = png.Encoder{CompressionLevel: png.BestSpeed}

func toNRGBA(imgSrc image.Image, bounds image.Rectangle) *image.NRGBA {
	switch src := imgSrc.(type) {
	case *image.NRGBA:
		return src
	case *image.RGBA:
		// Opaque RGBA (PNG without alpha): premultiplied == straight values,
		// so its buffer can be reinterpreted as NRGBA without copying.
		return &image.NRGBA{Pix: src.Pix, Stride: src.Stride, Rect: src.Rect}
	default:
		dst := image.NewNRGBA(bounds)
		draw.Draw(dst, bounds, imgSrc, bounds.Min, draw.Src)
		return dst
	}
}

func ImagePipeline(pathIn, pathOut string, op ImageOperation) error {
	fileIn, err := os.Open(pathIn)
	if err != nil {
		return fmt.Errorf("open %s: %w", pathIn, err)
	}
	defer fileIn.Close()

	imgSrc, _, err := image.Decode(fileIn)
	if err != nil {
		return fmt.Errorf("decode %s: %w", pathIn, err)
	}

	bounds := imgSrc.Bounds()
	w, h := bounds.Dx(), bounds.Dy()
	if w == 0 || h == 0 {
		return fmt.Errorf("empty image: %s", pathIn)
	}

	imgSrcNRGBA := toNRGBA(imgSrc, bounds)
	imgOutRGBA := image.NewNRGBA(bounds)

	op(
		(*C.uint8_t)(unsafe.Pointer(&imgSrcNRGBA.Pix[0])),
		(*C.uint8_t)(unsafe.Pointer(&imgOutRGBA.Pix[0])),
		w, h,
	)

	fileOut, err := os.Create(pathOut)
	if err != nil {
		return fmt.Errorf("create %s: %w", pathOut, err)
	}
	defer fileOut.Close()

	if err := pngEncoder.Encode(fileOut, imgOutRGBA); err != nil {
		return fmt.Errorf("encode %s: %w", pathOut, err)
	}
	return nil
}
