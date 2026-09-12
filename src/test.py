def lerp(a, b, c):
    return a + (b - a) * c

def parabolic( step,  max, p):
    x = step / max
    return 4 * x * (1 - x) ** p


for i in range(11):
    h = 10 / 2

    print(parabolic(i, 10, 4))