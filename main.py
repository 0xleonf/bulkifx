import bulkifxApp as bp

output_ = "gray_output.jpg"

img = bp.load_image("patrick.jpg")

img_gray = bp.convert_grayscale(img)

bp.save_image(output_, img_gray)
