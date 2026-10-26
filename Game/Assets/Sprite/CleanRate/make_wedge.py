from PIL import Image, ImageDraw, ImageChops

OUT = 300
SCALE = 4
WEDGE_DEG = 4.0

def square_crop(path):
    im = Image.open(path).convert("RGBA")
    a = im.split()[3]
    bbox = a.point(lambda v: 255 if v > 128 else 0).getbbox()
    print(path, "size:", im.size, "bbox:", bbox)
    l, t, r, b = bbox
    side = max(r - l, b - t)
    cx, cy = (l + r) / 2, (t + b) / 2
    box = (int(cx - side / 2), int(cy - side / 2),
           int(cx + side / 2), int(cy + side / 2))
    return im.crop(box).resize((OUT, OUT), Image.LANCZOS)

co = square_crop("Circular_Coating.png")
R, G, B, A = co.split()

# 水色の部分だけを色で判定
cyan = Image.new("L", (OUT, OUT), 0)
pr, pg, pb = R.load(), G.load(), B.load()
cp = cyan.load()
for y in range(OUT):
    for x in range(OUT):
        if pg[x, y] > 180 and pb[x, y] > 180 and pr[x, y] < 160:
            cp[x, y] = 255

# 扇形マスク
big = OUT * SCALE
mask = Image.new("L", (big, big), 0)
d = ImageDraw.Draw(mask)
d.pieslice([0, 0, big - 1, big - 1], start=-90, end=-90 + WEDGE_DEG, fill=255)
mask = mask.resize((OUT, OUT), Image.LANCZOS)

alpha = ImageChops.multiply(ImageChops.multiply(cyan, mask), A)
out = Image.merge("RGBA", (R, G, B, alpha))
out.save("Circular_Wedge.png")
print("done")
