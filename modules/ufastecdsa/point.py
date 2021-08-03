# coding=utf-8
# pylint: disable=E0401
import _crypto
from ufastecdsa.curve import Curve, P256


class CurveMismatchError(Exception):
    def __init__(self, curve1, curve2):
        self.msg = "Tried to add points on two different curves <{}> & <{}>".format(
            curve1.name, curve2.name
        )


class Point(object):
    def __init__(self, x, y, curve=P256):
        if not _crypto.ECC.Point(x, y, curve._curve) in curve._curve:
            raise ValueError("not on curve <{}>".format(curve.name))

        self._point = _crypto.ECC.Point(x, y, curve._curve)
        self._curve = Curve(
            self._point.curve.name,
            self._point.curve.p,
            self._point.curve.a,
            self._point.curve.b,
            self._point.curve.q,
            self._point.curve.gx,
            self._point.curve.gy,
            oid=self._point.curve.oid,
        )

    def __str__(self):
        return "<Point x=0x{:x} y=0x{:x} curve={!s}>".format(
            self._point.x, self._point.y, self._point.curve.name
        )

    def __repr__(self):
        return self.__str__()

    def __iter__(self):
        for item in (self._point.x, self._point.y):
            yield item

    def __eq__(self, other):
        return self._point == other._point

    def __add__(self, other):
        if self._point.curve != other._point.curve:
            raise CurveMismatchError(self._point.curve, other._point.curve)
        a = self._point + other._point
        return Point(a.x, a.y, curve=self._curve)

    def __radd__(self, other):
        return self.__add__(other)

    def __sub__(self, other):
        s = self._point - other._point
        return Point(s.x, s.y, curve=self._curve)

    def __mul__(self, scalar):
        m = self._point * scalar
        return Point(m.x, m.y, curve=self._curve)

    def __rmul__(self, scalar):
        return self.__mul__(scalar)

    def __neg__(self):
        return -self._point

    @property
    def x(self):
        return self._point.x

    @property
    def y(self):
        return self._point.y

    @property
    def curve(self):
        return self._curve

    def dumps(self, use_compression=False):
        if not use_compression:
            return (
                b"\x04"
                + self._point.x.to_bytes(32, "big")
                + self._point.y.to_bytes(32, "big")
            )
        elif self._point.y % 2 == 0:
            return b"\x02" + self._point.x.to_bytes(32, "big")
        else:
            return b"\x03" + self._point.x.to_bytes(32, "big")
