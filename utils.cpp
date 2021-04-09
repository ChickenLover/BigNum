#include "big.h"


BigInt BigInt::pow(BigInt a, BigInt power, BigInt modulus) {
    BigInt q(a);
    BigInt result = power.is_odd() ? a % modulus : 1;
    for(power = power >> 1; power > 0; power = power >> 1) {
        q = (q * q) % modulus;
        result = power.is_odd() ? (result * q) % modulus : result;
    }
    return result;
}

unsigned char hex_to_num(const unsigned char hex){
    int diff;
    if(hex > 47 && hex < 58)
        diff = 48;
    else if(hex > 64 && hex < 71)
        diff = 55;
    else if(hex > 96 && hex < 103)
        diff = 87;
    else
        throw std::invalid_argument("Incorrect hex!");
    return hex - diff;
}

char num_to_hex(const unsigned char num){
    if(num > 15)
        throw std::invalid_argument("Incorrect num!");
    if(num < 10)
        return num + 48;
    return num + 87;
}

BASE hex_to_base(const char *hex){
    BASE value = 0;
    int hex_len = strlen(hex);
    char hex_per_base = sizeof(BASE) * 2;
    int hex_multiplier = 1; // 8 * 10^0, 8 * 10^1 ... 8 * 10^n = hex_multiplier
    for(int i=hex_len; i>0; i--){
        int invert_index = hex_len - i;
        int base_index = invert_index / hex_per_base;
        value += hex_to_num(hex[i - 1]) * hex_multiplier;
        hex_multiplier = hex_multiplier * 16;
    }
    return value;
}

void base_hex(std::ostream &os, BASE number){
    if(!number){
        std::cout << "0";
    }
    int hex_per_base = sizeof(BASE) * 2;
    bool zeroes_stripped = false;
    int hex_multiplier = pow(16, hex_per_base - 1);
    for(int j=0; j<hex_per_base; j++){
        char symbol = num_to_hex(number / hex_multiplier);
        number %= hex_multiplier;
        hex_multiplier /= 16;
        if(!zeroes_stripped && symbol != '0'){
            zeroes_stripped = true;
        }
        if(!zeroes_stripped)
            continue;
        os << symbol;
    }
}

bool BigInt::is_even() const {
    return !this->length() || !((*this)[0] % 2);
}

bool BigInt::is_odd() const {
    return !this->is_even();
}

bool miller_rabin_check(BigInt d, BigInt n) {
    auto a = BigInt::random(2, n - 2);
    auto x = BigInt::pow(a, d, n);
    if (x == 1 || x == (n - 1) || x % n == 1) {
        return true;
    }
    while (d != (n - 1))
    {
        x = (x * x) % n;
        d = d << 1;
        if (x == 1) {
            return false;
        }
        if (x == n - 1) {
            return true;
        }
    }
    return false;
}

bool BigInt::is_prime() const {
    if (this->is_even()) return false;
    if (this->length() == 1) {
        auto n = (*this)[0];
        if (n == 0 || n == 1)
            return false;
        if (n == 2 || n == 3)
            return true;
    }

    auto d = (*this) - 1;
	while (d.is_even()) {
		d = d >> 1;
	}
	for (char i = 0; i < 10; i++) {
        if (!miller_rabin_check(d, (*this))) {
            return false;
        }
	}
    return true;
}


BigInt BigInt::next_prime() const {
    BigInt n = (*this) + (this->is_even() ? 1 : 2);
    while (!n.is_prime()) {
        n = n + 2;
    }
    return n;
}
