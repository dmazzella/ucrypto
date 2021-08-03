# coding=utf-8
# pylint: disable=E0401
import binascii
import hashlib

import _crypto
from ufastecdsa.curve import P256
from ufastecdsa.util import RFC6979, get_bit_length
from ufastecdsa.point import Point
from ufastecdsa.signature import Signature


class EcdsaError(Exception):
    def __init__(self, msg):
        self.msg = msg


class InvalidSignature(EcdsaError):
    def __init__(self, msg):
        self.msg = msg


def sign(msg, d, curve=P256, hashfunc=hashlib.sha256, nonce=None):
    k = nonce or RFC6979(msg, d, curve.q, hashfunc=hashfunc).gen_nonce()
    ks = k + curve.q
    kt = ks + curve.q
    if get_bit_length(ks) == get_bit_length(curve.q):
        k = kt
    else:
        k = ks
    digest = hashfunc(msg).digest()
    hex_digest = binascii.hexlify(digest)
    signature = _crypto.ECC.ecdsa_sign(hex_digest, d, k, curve._curve)
    return signature.r, signature.s


def verify(signature, message, Q, curve=P256, hashfunc=hashlib.sha256):
    if isinstance(signature, (tuple, list)):
        signature = Signature(signature[0], signature[1])
    if isinstance(signature, Signature):
        signature = _crypto.ECC.Signature(signature.r, signature.s)

    if not isinstance(Q, Point):
        raise EcdsaError("Invalid public key: point must be of type Point")

    if not Q._point in curve._curve:
        raise EcdsaError(
            "Invalid public key: point is not on curve {0}".format(curve.name)
        )
    elif signature.r > curve.q or signature.r < 1:
        raise InvalidSignature("r is not a positive int smaller than the curve order")
    elif signature.s > curve.q or signature.s < 1:
        raise InvalidSignature("s is not a positive int smaller than the curve order")

    digest = hashfunc(message).digest()
    hex_digest = binascii.hexlify(digest)
    return _crypto.ECC.ecdsa_verify(signature, hex_digest, Q._point, curve._curve)
