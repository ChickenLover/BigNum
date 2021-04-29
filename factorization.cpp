#include "big.h"


BigInt naive_division(BigInt n, BigInt b, std::vector<BigInt> &primes) {
    BigInt n_copy(n), r, q, k = 2, zero = BigInt((BASE)0);
    while (n_copy != 1) {
        q = n_copy.long_division(k, &r);
        if (r == zero) {
            primes.push_back(k);
            n_copy = q;
            continue;
        }
        if (k < q) {
            k = k.next_prime();
            if (k > b) {
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
