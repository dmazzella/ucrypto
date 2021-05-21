# coding=utf-8
# pylint: disable=E0401
import os

import _crypto
from ufastecdsa.curve import P256
from ufastecdsa.point import Point


def gen_private_key(curve=P256):
    order_bits = 0
    order = curve.q

    while order > 0:
        order >>= 1
        order_bits += 1

    order_bytes = (order_bits + 7) // 8
    extra_bits = order_bytes * 8 - order_bits

    rand = int.from_bytes(os.urandom(order_bytes), "big")
    rand >>= extra_bits

    while rand >= curve.q:
        rand = int.from_bytes(os.urandom(order_bytes), "big")
        rand >>= extra_bits

    return rand


def get_public_key(d, curve=P256):
    Q = d * curve.G
    return Point(Q.x, Q.y, curve=curve)


def gen_keypair(curve=P256):
    d = gen_private_key(curve)
    Q = get_public_key(d, curve)
    return d, Q
