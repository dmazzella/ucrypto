import hashlib
import hmac
import struct

try:
    int.bit_length(0)
    def get_bit_length(n):
        return n.bit_length()
except:
    # Work around
    def get_bit_length(n):
        i = 0
        while n:
            n >>= 1
            i += 1
        return i

class RFC6979(object):
    """Generate a nonce per RFC6979.

    In order to avoid reusing a nonce with the same key when signing two different messages (which
    leaks the private key) RFC6979 provides a deterministic method for generating nonces. This is
    based on using a pseudo-random function (HMAC) to derive a nonce from the message and private
    key. More info here: http://tools.ietf.org/html/rfc6979.

    Attributes:
        |  msg (str): A message being signed.
        |  x (int): An ECDSA private key.
        |  q (int): The order of the generator point of the curve being used to sign the message.
        |  hashfunc (_hashlib.HASH): The hash function used to compress the message.
    """

    def __init__(self, msg, x, q, hashfunc=hashlib.sha256):
        self.x = x
        self.q = q
        self.msg = msg
        self.qlen = get_bit_length(q)
        self.rlen = ((self.qlen + 7) // 8) * 8
        self.hashfunc = hashfunc

    def _bits2int(self, b):
        """ http://tools.ietf.org/html/rfc6979#section-2.3.2 """
        i = int.from_bytes(b, "big")
        blen = len(b) * 8
        if blen > self.qlen:
            i >>= blen - self.qlen
        return i

    def _int2octets(self, x):
        """ http://tools.ietf.org/html/rfc6979#section-2.3.3 """
        octets = b''
        while x > 0:
            octets = struct.pack('<B', (0xff & x)) + octets
            x >>= 8
        padding = b"\x00" * ((self.rlen // 8) - len(octets))
        return padding + octets

    def _bits2octets(self, b):
        """ http://tools.ietf.org/html/rfc6979#section-2.3.4 """
        z1 = self._bits2int(b)
        z2 = z1 % self.q
        return self._int2octets(z2)

    def gen_nonce(self):
        """ http://tools.ietf.org/html/rfc6979#section-3.2 """
        h1 = self.hashfunc(self.msg)
        hash_size = 32
        if hasattr(hashlib, "md5") and isinstance(h1, hashlib.md5):
            hash_size = 16
        if isinstance(h1, hashlib.sha1):
            hash_size = 20
        if isinstance(h1, hashlib.sha256):
            hash_size = 32
        h1 = h1.digest()
        key_and_msg = self._int2octets(self.x) + self._bits2octets(h1)
        v = b"\x01" * hash_size
        k = b"\x00" * hash_size

        k = hmac.new(k, v + b"\x00" + key_and_msg, self.hashfunc).digest()
        v = hmac.new(k, v, self.hashfunc).digest()

        k = hmac.new(k, v + b"\x01" + key_and_msg, self.hashfunc).digest()
        v = hmac.new(k, v, self.hashfunc).digest()

        while True:
            t = b""
            while len(t) * 8 < self.qlen:
                v = hmac.new(k, v, self.hashfunc).digest()
                t = t + v
            nonce = self._bits2int(t)
            if nonce >= 1 and nonce < self.q:
                return nonce
            k = hmac.new(k, v + b"\x00", self.hashfunc).digest()
            v = hmac.new(k, v, self.hashfunc).digest()


def test():
    from hashlib import sha1, sha256

    msg = "sample"
    x = 0x09A4D6792295A7F730FC3F2B49CBC0F62E862272F
    q = 0x4000000000000000000020108A2E0CC0D99F8A5EF

    expected = 0x09744429FA741D12DE2BE8316E35E84DB9E5DF1CD
    nonce = RFC6979(msg, x, q, sha1).gen_nonce()
    print("sha1", nonce == expected, hex(nonce))

    expected = 0x23AF4074C90A02B3FE61D286D5C87F425E6BDD81B
    nonce = RFC6979(msg, x, q, sha256).gen_nonce()
    print("sha256", nonce == expected, hex(nonce))

if __name__ == '__main__':
    test()
