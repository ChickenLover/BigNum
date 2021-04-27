#pragma once
#include <time.h>
#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <random>

#define EXTENSION_MUL 2

typedef unsigned int BASE;
typedef unsigned long DOUBLE_BASE;


class BigInt {
    BASE *al = NULL, *ar = NULL, *ah = NULL;

    void from_hex(const char *hex);
    char *to_hex() const;
    void extend_alloc();
    public:
        BigInt();
        BigInt(const char *chars);
        BigInt(const BigInt &other);
        BigInt(const BASE val);
        ~BigInt();

        void operator= (BASE other);
        BigInt &operator= (const BigInt &other);
        BASE &operator[] (const size_t index);
        BASE operator[] (const size_t index) const;

        BigInt operator+ (const BigInt &other) const;
        BigInt operator- (const BigInt &other) const;
        BigInt operator* (const BigInt &other) const;
        BigInt operator/ (const BigInt &other) const;
        BigInt operator% (const BigInt &other) const;
        BigInt operator<< (unsigned int shift) const;
        BigInt operator>> (unsigned int shift) const;

        BigInt &operator+= (const BigInt &other);
        BigInt &operator*= (const BASE &other);
        BigInt &operator/= (const BASE &other);
        BigInt &operator*= (const BigInt &other);
        BigInt &operator/= (const BigInt &other);

        bool operator< (const BigInt &other) const;
        bool operator> (const BigInt &other) const;
        bool operator<= (const BigInt &other) const;
        bool operator>= (const BigInt &other) const;
        bool operator== (const BigInt &other) const;
        bool operator!= (const BigInt &other) const;

        BASE operator% (BASE other) const;
        BigInt operator/ (const BASE &other) const;
        BigInt operator* (const BASE &other) const;

        bool operator< (const BASE &other) const;
        bool operator> (const BASE &other) const;
        bool operator>= (const BASE &other) const;
        bool operator<= (const BASE &other) const;
        bool operator== (const BASE &other) const;
        bool operator!= (const BASE &other) const;

        static BigInt random(BASE chunks);
        static BigInt with_cap(size_t capacity);
        static BigInt random(BigInt from, BigInt to);
        static BigInt random_prime(BigInt from, BigInt to);
        static BigInt pow(BigInt a, BigInt power, BigInt modulus);
        BigInt long_division(const BigInt &other, BigInt *reminder=NULL) const;
        BigInt division(const BASE &other, BASE *reminder_ptr=NULL) const;
        BigInt next_prime() const;
        bool is_prime() const;
        bool is_even() const;
        bool is_odd() const;
        void print_decimal();
        void zero();
        void push(const BASE new_el);
        BASE pop();
        void lstrip();
        size_t length() const;
        size_t capacity() const;

        friend std::ostream &operator<< (std::ostream &os, const BigInt &number);
        friend std::istream &operator>> (std::istream &is, BigInt &number);
        friend BigInt operator* (const BASE left, const BigInt &right);
};

unsigned char hex_to_num(const unsigned char hex);
char num_to_hex(const unsigned char num);
BASE hex_to_base(const char *hex);
void base_hex(std::ostream &os, BASE number);
