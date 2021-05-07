import hashlib

try:
    from ufastecdsa import curve, ecdsa, keys, util
    get_bit_length = util.get_bit_length
except ImportError:
    from fastecdsa import curve, ecdsa, keys, util
    get_bit_length = int.bit_length

def main():

    # private_key = 82378264402520040413352233063555671940555718680152892238371187003380781159101
    # public_key = keys.get_public_key(private_key, curve.P256)

    private_key, public_key = keys.gen_keypair(curve.P256)
    print("private_key:", private_key)
    print("public_key:", public_key.x, public_key.y, public_key.curve.name)

    m = "a message to sign via ECDSA"

    r, s = ecdsa.sign(m, private_key)

    print("R:", r)
    print("S:", s)

    verified = ecdsa.verify((r, s), m, public_key)
    print(verified)

if __name__ == "__main__":
    main()
