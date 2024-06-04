# Description

**Micropython package for doing fast rsa and elliptic curve cryptography, specifically digital signatures.
ECDSA API design inspired from [fastecdsa](https://github.com/AntonKueltz/fastecdsa) and implementation based on [tomsfastmath](https://github.com/libtom/tomsfastmath).**

> [!TIP]
> If you find **ucrypto** useful, consider :star: this project
> and why not ... [Buy me a coffee](https://www.buymeacoffee.com/damianomazp) :smile:

## Examples

- Signing and Verifying **ufastrsa**
    ```python
    from ufastrsa.rsa import RSA, genrsa


    def main():

        bits = 1024
        print("RSA bits", bits)
        r = RSA(*genrsa(bits, e=65537))
        if r:
            print("RSA OK")
            data = b"a message to sign and encrypt via RSA"
            print("random data len:", len(data), data)
            assert r.pkcs_verify(r.pkcs_sign(data)) == data
            print("pkcs_verify OK")
            assert r.pkcs_decrypt(r.pkcs_encrypt(data)) == data
            print("pkcs_decrypt OK")


    if __name__ == "__main__":
        main()
    ```

- Signing and Verifying **ufastecdsa**
    ```python
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
    ```

 - Arbitrary Elliptic Curve Arithmetic
    ```python
    from _crypto import ECC

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

    ```

- **for other examples:** [tests](https://github.com/dmazzella/ucrypto/tree/master/tests)

# Optimizations are disabled by **default** for easy build on different platforms
```c
#define TFM_NO_ASM

// #define TFM_ECC192
// #define TFM_ECC224
// #define TFM_ECC256
// #define TFM_ECC384
// #define TFM_ECC512
// #define TFM_RSA512
// #define TFM_RSA1024
// #define TFM_RSA2048
```

# Compiling the cmodule into MicroPython

To build such a module, compile MicroPython with an extra make flag named ```USER_C_MODULES``` set to the directory containing all modules you want included (not to the module itself).


- Example:
    ### PYBD_SF6
    ```bash
    ➜  ~ git clone https://github.com/micropython/micropython.git micropython
    ➜  ~ cd micropython
    ➜  micropython (master) ✗ git submodule update --init
    ➜  micropython (master) ✗ git clone https://github.com/dmazzella/ucrypto.git ports/stm32/boards/PYBD_SF6/cmodules/ucrypto
    ➜  micropython (master) ✗ make -j8 -C mpy-cross && make -j8 -C ports/stm32/ BOARD="PYBD_SF6" USER_C_MODULES="$(pwd)/ports/stm32/boards/PYBD_SF6/cmodules"
    ```
    ### ESP32_GENERIC
    ```bash
    ➜  ~ git clone https://github.com/micropython/micropython.git micropython
    ➜  ~ cd micropython
    ➜  micropython (master) ✗ git submodule update --init
    ➜  micropython (master) ✗ git clone https://github.com/dmazzella/ucrypto.git ports/esp32/boards/ESP32_GENERIC/cmodules/ucrypto
    ➜  micropython (master) ✗ make -j8 -C mpy-cross && make -j8 -C ports/esp32/ BOARD="ESP32_GENERIC" USER_C_MODULES="$(pwd)/ports/esp32/boards/ESP32_GENERIC/cmodules/ucrypto/micropython.cmake"
    ```
    ### ARDUINO_NANO_RP2040_CONNECT
    ```bash
    ➜  ~ git clone https://github.com/micropython/micropython.git micropython
    ➜  ~ cd micropython
    ➜  micropython (master) ✗ git submodule update --init
    ➜  micropython (master) ✗ git clone https://github.com/dmazzella/ucrypto.git ports/rp2/boards/ARDUINO_NANO_RP2040_CONNECT/cmodules/ucrypto
    ➜  micropython (master) ✗ make -j8 -C mpy-cross && make -j8 -C ports/rp2/ BOARD="ARDUINO_NANO_RP2040_CONNECT" USER_C_MODULES="$(pwd)/ports/rp2/boards/ARDUINO_NANO_RP2040_CONNECT/cmodules/ucrypto/micropython.cmake"
    ```

## Build size:

The build size depends on the asm optimizations of the tomsfastmath library that are enabled into ```ucrypto/tomsfastmath/tfm_mpi.h```
```c
#define TFM_ECC192
#define TFM_ECC224
#define TFM_ECC256
#define TFM_ECC384
#define TFM_ECC512
#define TFM_RSA512
#define TFM_RSA1024
#define TFM_RSA2048
```

- PYBD_SF6 without ucrypto:
    ```
    LINK build-PYBD_SF6/firmware.elf
    text	   data	    bss	    dec	    hex	filename
    1012856	    328	 100576	1113760	 10fea0	build-PYBD_SF6/firmware.elf
    ```
- PYBD_SF6 with ucrypto and with tomsfastmath only ECC 256 asm optimizations:
    ```c
    // #define TFM_ECC192
    // #define TFM_ECC224
    #define TFM_ECC256
    // #define TFM_ECC384
    // #define TFM_ECC512
    // #define TFM_RSA512
    // #define TFM_RSA1024
    // #define TFM_RSA2048
    ```
    ```
    LINK build-PYBD_SF6/firmware.elf
    text	   data	    bss	    dec	    hex	filename
    1034872	    452	 101600	1136924	 11591c	build-PYBD_SF6/firmware.elf
    ```
- PYBD_SF6 with ucrypto and without tomsfastmath RSA asm optimizations:
    ```c
    #define TFM_ECC192
    #define TFM_ECC224
    #define TFM_ECC256
    #define TFM_ECC384
    #define TFM_ECC512
    // #define TFM_RSA512
    // #define TFM_RSA1024
    // #define TFM_RSA2048
    ```
    ```
    LINK build-PYBD_SF6/firmware.elf
    text	   data	    bss	    dec	    hex	filename
    1042552	    452	 101600	1144604	 11771c	build-PYBD_SF6/firmware.elf
    ```
- PYBD_SF6 with ucrypto and full tomsfastmath asm optimizations:
    ```
    LINK build-PYBD_SF6/firmware.elf
    text	   data	    bss	    dec	    hex	filename
    1209976	    452	 101600	1312028	 14051c	build-PYBD_SF6/firmware.elf
    ```

To see which optimizations are enabled in the build:
```python
MicroPython v1.19.1-705-gac5934c96-dirty on 2022-11-22; PORTENTA with STM32H747
Type "help()" for more information.
>>> import _crypto
>>> print(_crypto.NUMBER.ident())
TomsFastMath v0.13.1-next

Sizeofs
        fp_digit = 4
        fp_word  = 8

FP_MAX_SIZE = 4352

Defines: 
 TFM_ARM  TFM_ECC192  TFM_ECC224  TFM_ECC256  TFM_ECC384  TFM_ECC512  TFM_RSA512  TFM_RSA1024  TFM_RSA2048  TFM_ASM  TFM_MUL6  TFM_SQR6  TFM_MUL7  TFM_SQR7  TFM_MUL8  TFM_SQR8  TFM_MUL12  TFM_SQR12  TFM_SMALL_SET  TFM_MUL17  TFM_SQR17  TFM_MUL32  TFM_SQR32  TFM_MUL64  TFM_SQR64 

>>>
```
