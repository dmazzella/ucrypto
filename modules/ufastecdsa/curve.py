# coding=utf-8
# pylint: disable=E0401
import _crypto


class CurveNotFoundError(Exception):
    def __init__(self, oid):
        self.msg = "{0} is not found. Supported named curves are: {1}".format(
            oid, _CURVE_OIDS.keys()
        )


class Curve(object):
    def __init__(self, name, p, a, b, q, gx, gy, oid=None):
        self._curve = _crypto.ECC.Curve(p, a, b, q, gx, gy, name=name, oid=oid)

    @classmethod
    def from_oid(cls, oid):
        if not oid in _CURVE_OIDS:
            raise CurveNotFoundError(oid)

        return _CURVE_OIDS[oid]

    def __getattr__(self, name):
        if name in ("p", "a", "b", "q", "gx", "gy", "name", "oid", "G"):
            return getattr(self._curve, name)
        raise AttributeError(name)

    def __str__(self):
        return "<Curve p=0x{:x} a=0x{:x} b=0x{:x} q=0x{:x} gx=0x{:x} gy=0x{:x} name={:s} oid={:s}>".format(
            self._curve.p,
            self._curve.a,
            self._curve.b,
            self._curve.q,
            self._curve.gx,
            self._curve.gy,
            self._curve.name,
            self._curve.oid,
        )

    def __repr__(self):
        return self.__str__()

    def __contains__(self, P):
        return P._point in self._curve

    def __iter__(self):
        for item in (
            self._curve.p,
            self._curve.a,
            self._curve.b,
            self._curve.q,
            self._curve.gx,
            self._curve.gy,
            self._curve.name,
            self._curve.oid,
        ):
            yield item

P256 = Curve(
    "P256",
    0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF,
    0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC,
    0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B,
    0xFFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551,
    0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296,
    0x4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5,
    oid=b'\x2A\x86\x48\xCE\x3D\x03\x01\x07',
)

_CURVE_OIDS = {
    b'\x2A\x86\x48\xCE\x3D\x03\x01\x07': P256
}
