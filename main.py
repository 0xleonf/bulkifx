import sys

from ImageProc import Grayscale, Sepia

if __name__ == "__main__":
    # if len(sys.argv) != 3:
    #     sys.exit("usage: python3 main.py <input> <output>")
    # Grayscale(sys.argv[1], sys.argv[2])
    input = "images.jpg"
    Sepia(input, "output.png")
