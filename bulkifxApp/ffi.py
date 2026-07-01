import ctypes
import os
import sys

_curr_dir = os.path.dirname(os.path.abspath(__file__))

_so_name = "libbulkifx.so"
_path_so = os.path.join(_curr_dir, _so_name)

if not os.path.exists(_path_so):
    _path_so = os.path.abspath(os.path.join(_curr_dir, "../build", _so_name))

if not os.path.exists(_path_so):
    raise ImportError(
        f"Binary {_path_so} not found"
        f"make sure compiled C have done")

lib = ctypes.CDLL(_path_so)


class Image(ctypes.Structure):
    _fields_ = [
        ("R", ctypes.POINTER(ctypes.c_uint8)),
        ("G", ctypes.POINTER(ctypes.c_uint8)),
        ("B", ctypes.POINTER(ctypes.c_uint8)),
        ("data", ctypes.POINTER(ctypes.c_uint8)),
        ("width", ctypes.c_int),
        ("height", ctypes.c_int),
        ("channels", ctypes.c_int)
    ]


ImagePtr = ctypes.POINTER(Image)

# core. c
lib.load_image.argtypes = [ctypes.c_char_p]
lib.load_image.restype = ImagePtr

lib.image_print_info.argtypes = [ImagePtr]
lib.image_print_info.restype = None

lib.save_image.argtypes = [ctypes.c_char_p, ImagePtr]
lib.save_image.restype = None

lib.free_image.argtypes = [ImagePtr]
lib.free_image.restype = None

# img_proc.c
lib.grayscale.argtypes = [ImagePtr]
lib.grayscale.restype = ImagePtr
