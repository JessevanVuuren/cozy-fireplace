def lerp(a, b, c):
    return a + (b - a) * c

def parabolic( step,  max, p):
    x = step / max
    return 4 * x * (1 - x) ** p


for i in range(11):
    h = 10 / 2

    print(parabolic(i, 10, 4))


from bdfparser import Font

font = Font('MatrixLight8x6.bdf')

# ac = font.glyph("A").draw().crop(5, 8, 0, 0)
# print(ac)

string = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz!\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}~"
# string = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

for c in string:
    glyph = font.glyph(c).draw()
    print()
    print("// " + c)
    for row in glyph.todata():
        print(row)
        # h = hex(row)

        # if row == 0:
        #     print("0x00,")
        # elif len(h) ==3:
        #     print("0x0" + str(h[len(str(h))-1]).upper() + ",")
        # else:
        #     print("0x" + str(h[2:]).upper() + ",")


