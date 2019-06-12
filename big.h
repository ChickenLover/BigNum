#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <math.h>

#define EXTENSION_MUL 2
#define BASE unsigned int
#define DOUBLE_BASE unsigned long


class BigInt {
    BASE *al = NULL, *ar = NULL, *ah = NULL;

    void from_hex(const char *hex);
    char *to_hex() const;
    void extend_alloc();
    public:
        BigInt();
        BigInt(const char *chars);
        BigInt(const BigInt &other);
        BigInt(const size_t capacity);
        ~BigInt();
        
        BigInt &operator= (const BigInt &other);
        BASE &operator[] (const size_t index);
        BASE operator[] (const size_t index) const;

        BigInt operator+ (const BigInt &other) const;
        BigInt operator- (const BigInt &other) const;
        BigInt operator* (const BigInt &other) const;
        BigInt operator* (const BASE &other) const;
        BigInt operator/ (const BigInt &other) const;
        BigInt operator/ (const BASE &other) const;
        BASE operator% (BASE other) const;
        BigInt operator% (const BigInt &other) const;
        BigInt operator<< (const char shift) const;
        BigInt operator>> (const char shift) const;

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

        BigInt long_division(const BigInt &other, BigInt *reminder=NULL) const;
        BigInt division(const BASE &other, BASE *reminder_ptr=NULL) const;
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
