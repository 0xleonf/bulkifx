import sys

from ImageProc import *

if __name__ == "__main__":
    # if len(sys.argv) != 3:
    #     sys.exit("usage: python3 main.py <input> <output>")
    # Grayscale(sys.argv[1], sys.argv[2])
    input = "width_412.jpg"
    Sepia(input, "output_sep.jpg")
