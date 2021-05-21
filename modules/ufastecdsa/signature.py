# coding=utf-8
# pylint: disable=E0401
import _crypto


class Signature(object):
    def __init__(self, r, s):
        self._r = 0
        self._s = 0
        if isinstance(r, int):
            self._r = r
        elif isinstance(r, (bytes, bytearray)):
            self._r = int.from_bytes(r, "big")
        else:
            raise TypeError("r must be a int, bytes or bytearray")

        if isinstance(s, int):
            self._s = s
        elif isinstance(s, (bytes, bytearray)):
            self._s = int.from_bytes(s, "big")
        else:
            raise TypeError("s must be a int, bytes or bytearray")

        self._signature = _crypto.ECC.Signature(self._r, self._s)

    @property
    def r(self):
        return self._signature.r

    @property
    def s(self):
        return self._signature.s

    def __str__(self):
        return "<Signature r=0x{:x} s=0x{:x}>".format(
            self._signature.r, self._signature.s
        )

    def __unicode__(self):
        return self.__str__()

    def __repr__(self):
        return self.__str__()
