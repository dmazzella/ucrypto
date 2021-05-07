try:
    if "urandom" not in globals():
        import sys
        if sys.platform == 'linux':
            def urandom(size):
                with open("/dev/urandom", "rb") as f:
                    return f.read(size)
        else:
            from urandom import getrandbits
            def urandom(size):
                try:
                    return bytes(getrandbits(8) for i in range(size))
                except ImportError as exc:
                    raise exc

    from ubinascii import hexlify, unhexlify
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
    0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5,
    name='P256',
    oid="2a8648ce3d030107" # b'\x2A\x86\x48\xCE\x3D\x03\x01\x07'
)

def gen_keypair(curve):
    private_key = gen_private_key(curve)
    public_key = get_public_key(private_key, curve)
    return private_key, public_key

def gen_private_key(curve):
    order_bits = 0
    order = curve.q

    while order > 0:
        order >>= 1
        order_bits += 1

    order_bytes = (order_bits + 7) // 8  # urandom only takes bytes
    extra_bits = order_bytes * 8 - order_bits  # bits to shave off after getting bytes

    rand = int.from_bytes(urandom(order_bytes), 'big')
    rand >>= extra_bits

    # no modding by group order or we'll introduce biases
    while rand >= curve.q:
        rand = int.from_bytes(urandom(order_bytes), 'big')
        rand >>= extra_bits

    return rand

def get_public_key(d, curve):
    return d * curve.G

_TEST_KEY = {
    "PRIVATE": bytes([
        0XF3, 0XFC, 0XCC, 0X0D, 0X00, 0XD8, 0X03, 0X19, 0X54, 0XF9, 0X08, 0X64, 0XD4, 0X3C, 0X24, 0X7F,
        0X4B, 0XF5, 0XF0, 0X66, 0X5C, 0X6B, 0X50, 0XCC, 0X17, 0X74, 0X9A, 0X27, 0XD1, 0XCF, 0X76, 0X64
    ]),
    "PUBLIC": bytes([
        0X8D, 0X61, 0X7E, 0X65, 0XC9, 0X50, 0X8E, 0X64, 0XBC, 0XC5, 0X67, 0X3A, 0XC8, 0X2A, 0X67, 0X99,
        0XDA, 0X3C, 0X14, 0X46, 0X68, 0X2C, 0X25, 0X8C, 0X46, 0X3F, 0XFF, 0XDF, 0X58, 0XDF, 0XD2, 0XFA,
        0X3E, 0X6C, 0X37, 0X8B, 0X53, 0XD7, 0X95, 0XC4, 0XA4, 0XDF, 0XFB, 0X41, 0X99, 0XED, 0XD7, 0X86,
        0X2F, 0X23, 0XAB, 0XAF, 0X02, 0X03, 0XB4, 0XB8, 0X91, 0X1B, 0XA0, 0X56, 0X99, 0X94, 0XE1, 0X01
    ])
}

def test():
    private_key = int.from_bytes(_TEST_KEY["PRIVATE"], 'big')
    public_key = get_public_key(private_key, P256)
    print("PRV KEY: {:x}".format(private_key))
    print("PUB KEY: 04{:x}{:x}".format(public_key.x, public_key.y))


if __name__ == "__main__":
    test()
