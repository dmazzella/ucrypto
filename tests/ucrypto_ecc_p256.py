try:
    from _crypto import ECC
except ImportError:
    print("SKIP")
    raise SystemExit

P256 = ECC.Curve(
    0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff,
    -0x3,
    0x5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b,
    0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551,
    0x6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296,
    0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5
)

S = ECC.Point(
    0xde2444bebc8d36e682edd27e0f271508617519b3221a8fa0b77cab3989da97c9,
    0xc093ae7ff36e5380fc01a5aad1e66659702de80f53cec576b6350b243042a256,
    P256
)

T = ECC.Point(
    0x55a8b00f8da1d44e62f6b3b25316212e39540dc861c89575bb8cf92e35e0986b,
    0x5421c3209c2d6c704835d82ac4c3dd90f61a8a52598b9e7ab656e9d8c8b24316,
    P256
)

print("S==S  = ", S == S)

print("S==T  = ", S == T)

R = S + T
print("S+T   = ({:X}, {:X})".format(R.x, R.y))

R = S - T
print("S-T   = ({:X}, {:X})".format(R.x, R.y))

R = 2 * S
print("2S    = ({:X}, {:X})".format(R.x, R.y))

d = 0xc51e4753afdec1e6b6c6a5b992f43f8dd0c7a8933072708b6522468b2ffb06fd
e = 0xd37f628ece72a462f0145cbefe3f0b355ee8332d37acdd83a358016aea029db7
R = (d * S) + (e * T)
print("dS+eT = ({:X}, {:X})".format(R.x, R.y))

R = S + S
print("S+S   = ({:X}, {:X})".format(R.x, R.y))

R = S - S
print("S-S   = ({:X}, {:X})".format(R.x, R.y))
