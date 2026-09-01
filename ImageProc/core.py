from .ffi import lib


def Grayscale(input: str, output: str):
    status = lib.Grayscaling(input.encode("utf-8"), output.encode("utf-8"))
    if status != 0:
        raise RuntimeError(
            "C function Grayscaling failed (status 1), check stderr for details"
        )
    return status
