import bulkifxApp as bp

output_ = "gray_output.jpg"

img = bp.load_image("spon.jpg")

img_gray = bp.convert_grayscale(img)
sepia_img = bp.convert_sepia(img)

bp.save_image(output_, img_gray)
bp.save_image("sepia_output.jpg", sepia_img)
bp.free_image(img)
