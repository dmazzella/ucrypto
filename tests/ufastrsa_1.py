from time import ticks_diff, ticks_ms

from ufastrsa.rsa import RSA, genrsa


def main():

    bits = 1024
    print("RSA bits", bits)
    start = ticks_ms()
    r = RSA(*genrsa(bits, e=65537))
    if r:
        end = ticks_ms()
        print(ticks_diff(end, start))
        print("RSA OK")
        data = b"a message to sign and encrypt via RSA"
        print("random data len:", len(data), data)
        start = ticks_ms()
        assert r.pkcs_verify(r.pkcs_sign(data)) == data
        end = ticks_ms()
        print(ticks_diff(end, start))
        print("pkcs_verify OK")
        start = ticks_ms()
        assert r.pkcs_decrypt(r.pkcs_encrypt(data)) == data
        end = ticks_ms()
        print(ticks_diff(end, start))
        print("pkcs_decrypt OK")


if __name__ == "__main__":
    main()
