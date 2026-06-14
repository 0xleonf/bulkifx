import bulkifxApp as bp


output_ = "gray_output.jpg"

img = bp.load_image("spon.jpg")

img_gray = bp.convert_grayscale(img)

bp.free_image(img)
bp.save_image(output_, img_gray)

bp.free_image(img_gray)