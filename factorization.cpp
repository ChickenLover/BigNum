#include "big.h"


BigInt _f(BigInt x, BigInt n) {
    return (x * x + 1) % n;
}

BigInt pollard(BigInt n) {
    while (n.is_even()) {
        n = n >> 1;
    }
	BigInt a = 2, b = 2;
	BigInt d = 1;
	while (d == 1) {
		a = _f(a, n), b = _f(_f(b, n), n);
		if (a == b) {
			return BigInt((BASE)0);
		}
		d = BigInt::gcd(a > b ? a - b : b - a, n);
	}
	return d;
}

/*
BN Pollard2_inner(BN n, BN b) {
	auto a = BN::rand(2, n - 2);
	auto d = BN::gcd(a, n);
	if (d > 1) {
		return d;
	}
	BN q_i = 2;
	while (q_i <= b) {
		auto e_i = BN::floorLog(n, q_i);
		a = BN::pow(a, BN::pow(q_i, e_i, n), n);
		if (a == BN(1)) {
			return BN(0);
		}
		d = BN::gcd(a - 1, d);
		if (d > 1) {
			return d;
		}
		q_i = q_i.nextPrime();
	}
	return 1;
}


BN Pollard2(BN n, BN b) {
	assert(!n.isEven());
	assert(!n.isPrime());
	assert(b > 1);
	BN result;
	for (char i = 0; i < 100 && result == 0; i++) {
		result = Pollard2_inner(n, b);
	}
	return result;
}
*/

std::pair<BigInt, BigInt> fermat(BigInt n) {
    while (n.is_even()) {
        n = n >> 1;
    }
	BigInt x = n.floor_root(2);
	if (x * x == n) {
		return std::make_pair(x, x);
	}
	BigInt max = (n + 9) / (BASE)6;
    BigInt z = (x + 1) * (x + 1) - n;
	while ((x = x + 1) <= max) {
		BigInt y = z.floor_root(2);
		if (y * y == z) {
			return std::make_pair(x + y, x - y);
		}
        z = z + ((x + 1) * 2) - 1;
	}
	return std::make_pair(0, 0);
}


BigInt naive_division(BigInt n, BigInt b, std::vector<BigInt> &primes) {
    BigInt n_copy(n), r, q, d = 2;
    while (n_copy != 1) {
        q = n_copy.long_division(d, &r);
        if (r.is_zero()) {
            primes.push_back(d);
            n_copy = q;
            continue;
        }
        if (d < q) {
            d = d.next_prime();
            if (d > b) {
                break;
            }
            continue;
        } else {
            primes.push_back(n);
            break;
        }
    }
    return n_copy;
}


BigInt olvey(BigInt n) {
    while (n.is_even()) {
        n = n >> 1;
    }
	BigInt d = (n.floor_root(3) << 1) + 1; // d всегда положительное нечетное
    BigInt r1, r2;
    BigInt d1 = n.long_division(d, &r1);
    BigInt d2 = n.long_division(d - 2, &r2);
	BigInt q = (d2 - d1) * 4;
    BigInt s = n.floor_root(2);
	while ((d = d + 2) <= s) {
		BigInt r = (r1 << 1) + q;
		if (r2 >= r) {
			r = r + d;
			q = q + 4;
		}
		r = r - r2;
		while (r >= d) {
			r = r - d;
			q = q - 4;
		}
		if (r.is_zero()) {
			return d;
		}
		else {
			r2 = r1;
			r1 = r;
		}
	}
	return BigInt((BASE)0);
}
