from .ffi import lib


def Grayscale(input: str, output: str):
    status = lib.Grayscaling(input.encode("utf-8"), output.encode("utf-8"))
    if status != 0:
        raise RuntimeError(
            "C function Grayscaling failed (status 1), \
            check stderr for details"
        )
    return status


def Sepia(input: str, output: str):
    status = lib.Sepia(input.encode("utf-8"), output.encode("utf-8"))
    if status != 0:
        raise RuntimeError(
            "C function Sepia failed (status 1), check stderr for details"
        )
    return status


def ZeroPadding(input: str, output: str, *, padding: int = 1):
    status = lib.ZeroPadding(input.encode(
        "utf-8"), output.encode("utf-8"), padding)
    if status != 0:
        raise RuntimeError(
            "C function ZeroPadding failed (status 1), check stderr for details"
        )
    return status
