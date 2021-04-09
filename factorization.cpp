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
