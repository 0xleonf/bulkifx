import ctypes
import os

_curr_dir = os.path.dirname(os.path.abspath(__file__))

_so_name = "bulkifx.so"
_path_so = os.path.join(_curr_dir, _so_name)

if not os.path.exists(_path_so):
    _path_so = os.path.abspath(os.path.join(_curr_dir, "../build", _so_name))

if not os.path.exists(_path_so):
    raise ImportError(
        f"Binary {_path_so} not found. "
        "Make sure build.sh has been executed."
    )

lib = ctypes.CDLL(_path_so)

lib.Grayscaling.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
lib.Grayscaling.restype = ctypes.c_int

lib.Sepia.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
lib.Sepia.restype = ctypes.c_int
