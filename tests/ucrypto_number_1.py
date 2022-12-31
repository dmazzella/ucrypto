import random
import sys

from time import ticks_diff, ticks_ms

try:
    from _crypto import NUMBER as tomsfastmath
except ImportError:
    print("SKIP")
    raise SystemExit

print(tomsfastmath.ident())

################################################################################

x1 = 94503811836633892797439052099974064715915610386195161673505774830642447338297411034167323482825523579967763955917702346493755256033689254797842978878802566357722424464119547719373276634040210660014247381783837077060028495333630655215694495166834990272196064420249377634078368514964957906785144546175264297266
y1 = 71074765520811535379138799701268298623992777006974550285436363554368439313927061208425013564629734478853799202093051832216416876398353508688951125285138098211133917836653653008704451186463461221420093373376331037115859563141818234980194754180338501042765827141801188316330168083520295124883343213843145975085
z1 = 142149531041623070758277599402536597247985554013949100570872727108736878627854122416850027129259468957707598404186103664432833752796707017377902250570276196422267835673307306017408902372926922442840186746752662074231719126283636469960389508360677002085531654283602376632660336167040590249766686427686291950171


def fast_pow(x, e, m):
    return tomsfastmath.fast_pow(x, e, m)


def pow3(x, y, z):
    ans = 1
    while y:
        if y & 1:
            ans = (ans * x) % z
        y >>= 1
        if not y:
            break
        x = (x * x) % z
    return ans


def exptmod(x, y, z, safe=True):
    return tomsfastmath.exptmod(x, y, z, safe)


for i in range(0, 5):
    start = ticks_ms()
    print(exptmod(x1, y1, z1))
    end = ticks_ms()
    print("exptmod", ticks_diff(end, start))
    start = ticks_ms()
    print(fast_pow(x1, y1, z1))
    end = ticks_ms()
    print("fast_pow", ticks_diff(end, start))
    start = ticks_ms()
    print(pow(x1, y1, z1))
    end = ticks_ms()
    print("pow", ticks_diff(end, start))
    start = ticks_ms()
    print(pow3(x1, y1, z1))
    end = ticks_ms()
    print("pow3", ticks_diff(end, start))

    x1 = random.randint(0, sys.maxsize)
    y1 = random.randint(0, sys.maxsize)
    z1 = random.randint(0, sys.maxsize)

################################################################################


def invmod(a, b):
    return tomsfastmath.invmod(a, b)


start = ticks_ms()
print(invmod(x1, y1))
end = ticks_ms()
print("invmod", ticks_diff(end, start))

################################################################################


def generate_prime(num=1024, test=25, safe=False):
    return tomsfastmath.generate_prime(num, test, safe)


start = ticks_ms()
p = generate_prime(1024)
print(p)
end = ticks_ms()
print("generate_prime", ticks_diff(end, start))


def miller_rabin_test(n, test=25):
    return tomsfastmath.is_prime(n, test)


start = ticks_ms()
print(miller_rabin_test(p))
end = ticks_ms()
print("miller_rabin_test", ticks_diff(end, start))

################################################################################


def gcd(a, b):
    return tomsfastmath.gcd(a, b)


start = ticks_ms()
print(gcd(x1, y1))
end = ticks_ms()
print("gcd", ticks_diff(end, start))

################################################################################
