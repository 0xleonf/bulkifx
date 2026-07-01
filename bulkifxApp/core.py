from .ffi import lib, Image, ImagePtr


def load_image(filename: str) -> ImagePtr:
    """
    Load images using C binary
    """
    if not isinstance(filename, str):
        raise TypeError("Filename must be string (str)")

    filename_bytes = filename.encode("utf-8")

    img_ptr = lib.load_image(filename_bytes)
    if not img_ptr:
        raise FileNotFoundError(f"Failed to load image: {filename}")

    return img_ptr


def save_image(filename: str, img_ptr: ImagePtr) -> None:
    if not img_ptr:
        raise ValueError("Pointer image is NULL")

    filename_bytes = filename.encode("utf-8")
    lib.save_image(filename_bytes, img_ptr)
    lib.free_image(img_ptr)


def convert_grayscale(img_ptr: ImagePtr) -> ImagePtr:
    """
    Convert image to grayscale.
    """
    if not img_ptr:
        raise ValueError("Pointer source image NULL.")

    img_gray_ptr = lib.grayscale(img_ptr)
    if not img_gray_ptr:
        raise RuntimeError("Failed to process from binary C.")

    return img_gray_ptr


def free_image(img_ptr: ImagePtr) -> None:
    """
    Freed memory allocation in C.
    """
    if img_ptr:
        lib.free_image(img_ptr)
