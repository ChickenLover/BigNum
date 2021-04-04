#include "big.h"


BigInt BigInt::operator+ (const BigInt &other) const{
    const BigInt bigger = other.length() > this->length() ? other : *this;
    const BigInt smaller = other.length() <= this->length() ? other : *this;
    BigInt new_big_int = BigInt::with_cap(bigger.length() + 1);
    DOUBLE_BASE carry = 0;
    for(int i=0; i<bigger.length() + 1; i++){
        DOUBLE_BASE result = (DOUBLE_BASE)(i < smaller.length() ? smaller[i] : 0)\
                             + (DOUBLE_BASE)(i < bigger.length() ? bigger[i] : 0)\
                             + carry;
        if(result || i < bigger.length())
            new_big_int.push((BASE)result);
        carry = result >> (sizeof(BASE) * 8);
    }
    return new_big_int;
}

BigInt BigInt::operator- (const BigInt &other) const{
    BigInt new_big_int = BigInt::with_cap(this->length());
    bool carry = 0;
    for(int i=0; i<this->length(); i++){
        DOUBLE_BASE result = ((DOUBLE_BASE)1 << (sizeof(BASE) * 8)) + (*this)[i]
                             - carry
                             - (i < other.length() ? other[i] : 0);
        carry = !(result >> (sizeof(BASE) * 8));
        new_big_int.push((BASE)result);
    }
    new_big_int.lstrip();
    return new_big_int;
}

BigInt BigInt::operator* (const BASE &other) const{
    BigInt new_big_int = BigInt::with_cap(this->length() + 1);
    DOUBLE_BASE carry = 0;
    for(int i=0; i<this->length(); i++){
        DOUBLE_BASE result = (DOUBLE_BASE)(*this)[i] * other + carry;
        new_big_int.push((BASE)result);
        carry = result >> (sizeof(BASE) * 8);
    }
    if(carry)
        new_big_int.push((BASE)carry);
    return new_big_int;
}

BigInt operator* (const BASE left, const BigInt &right){
    return right * left;
}

BigInt BigInt::operator* (const BigInt &other) const{
    const BigInt bigger = other.length() > this->length() ? other : *this;
    const BigInt smaller = other.length() <= this->length() ? other : *this;
    BigInt new_big_int = BigInt::with_cap(bigger.length() + smaller.length());
    for(int i=0; i<new_big_int.capacity(); i++){
        new_big_int.push(0);
    }
    for(int i=0; i<smaller.length(); i++){
        DOUBLE_BASE carry = 0;
        for(int j=0; j<bigger.length(); j++){
            DOUBLE_BASE result = (DOUBLE_BASE)smaller[i]\
                                 * (DOUBLE_BASE)bigger[j]\
                                 + new_big_int[i + j]\
                                 + carry;
            new_big_int[i + j] = (BASE)result;
            carry = result >> (sizeof(BASE) * 8);
        }
        if(carry)
            new_big_int[i + bigger.length()] = (BASE)carry;
    }
    new_big_int.lstrip();
    return new_big_int;
}

BigInt BigInt::division(const BASE &other, BASE* reminder_ptr) const{
    if(!other)
        throw std::invalid_argument("Division by zero!");
    if(!this->length()){
        if(reminder_ptr)
            *reminder_ptr = 0;
        return BigInt();
    }
    BigInt result_int = BigInt::with_cap(this->length());
    for(int j=0; j<result_int.capacity(); j++){
        result_int.push(0);
    }
    DOUBLE_BASE carry = 0;
    for(int i=this->length() - 1; i>=0; i--){
        DOUBLE_BASE u = (carry << (sizeof(BASE) * 8)) + (*this)[i];
        BASE result = u / other;
        carry = u % other;
        result_int[i] = result;
    }
    if(reminder_ptr)
        *reminder_ptr = carry;
    result_int.lstrip();
    return result_int;
}

BigInt BigInt::operator/ (const BASE &other) const{
    return this->division(other);
}

BASE BigInt::operator% (BASE other) const{
    BASE *reminder = new BASE;
    this->division(other, reminder);
    BASE rem_copy = *reminder;
    delete reminder;
    return rem_copy;
}

BigInt BigInt::long_division(const BigInt &other, BigInt *reminder_ptr) const{
    if(reminder_ptr)
        reminder_ptr->zero();
    if(this->length() < other.length()
       || (this->length() == other.length() && (*this) < other)){
        if(reminder_ptr)
            *reminder_ptr = (*this);
        return BigInt();
    }
    int n = other.length();
    if(n == 1){
        if(reminder_ptr)
            reminder_ptr->push((*this) % other[n - 1]);
        return (*this) / other[n - 1];
    }
    BASE b = (BASE)(-1);
    int m = this->length() - n;
    BigInt result = BigInt::with_cap(m + 1);
    for(int i=0; i<m+1; i++)
        result.push(0);

    // Normalize ==================================================================
    BASE d = ((DOUBLE_BASE)b + 1) / (other[n - 1] + 1);
    //std::cout << "D is " << d << std::endl;
    BigInt v = other * d;
    BigInt u = (*this) * d;
    //std::cout << "V is " << other << ", Normalized V " << v << std::endl;
    //std::cout << "U is " << (*this) << ", Normalized U " << u << std::endl;
    // ============================================================================

    for(int j=m; j>=0; j--){
        // Calculate q ============================================================================
        DOUBLE_BASE two_upper_chunks = ((DOUBLE_BASE)u[j + n] << (sizeof(BASE) * 8))
                                       + u[j + n - 1];
        //std::cout << "Two upper chunks " << two_upper_chunks << std::endl;
        DOUBLE_BASE q = two_upper_chunks / v[n - 1];
        //std::cout << "q" << j << " is " << q << std::endl;
        DOUBLE_BASE r = two_upper_chunks % v[n - 1];
        //std::cout << "r" << j << " is " << r << std::endl;
        if(q - 1 == b || (q * v[n - 2] > ((r << (sizeof(BASE) * 8)) + u[j + n - 2]))){
            q--;
            r += v[n - 1];
            //std::cout << "Decreasing q, q" << j << " is now " << q << " r is now " << r << std::endl;
            //if(r > b)
            //    break;
        }
        //=======================================================================================

        // Mul and sub ==========================================================================
        bool bad_result = false;
        BigInt upper_part = BigInt::with_cap(n + 2);
        for(int i=0; i<=n; i++)
            upper_part.push(u[j + i]);
        BigInt smaller((BASE)q * v);
        if(smaller > upper_part){
            upper_part.push(1);
            bad_result = true;
        }
        BigInt reminder = upper_part - smaller;
        //std::cout << upper_part << " - " << q << " * " << v << " = " << reminder << std::endl;
        for(int i=0; i<=n; i++)
            u[j + i] = i < reminder.length() ? reminder[i] : 0;
        //std::cout << "U is now " << u << std::endl;
        // ======================================================================================

        // Check reminder ========================
        result[j] = (BASE)q;
        if(bad_result){
            //std::cout << "COMPENSATED" << std::endl;
            result[j]--;
            BigInt slice;
            for(int i=0; i<=n; i++){
                slice.push(u[i + j]);
            }
            BigInt compensation = slice + v;
            for(int i=0; i<=n; i++){
                u[i + j] = compensation[i];
            }
        }
        // ======================================
    }
    if(reminder_ptr)
        *reminder_ptr = u / d;
    result.lstrip();
    return result;
}

BigInt BigInt::operator/ (const BigInt &other) const{
    return this->long_division(other);
}

BigInt BigInt::operator% (const BigInt &other) const{
    BigInt reminder;
    this->long_division(other, &reminder);
    return reminder;
}

BigInt BigInt::operator>> (unsigned int shift) const {
    auto words_to_del = shift / (sizeof(BASE) * 8);
    if (!this->length() || words_to_del >= this->length()) {
        return BigInt((BASE)0);
    }
    int part_shift = shift % (sizeof(BASE) * 8);
    int hight_part_shift = (sizeof(BASE) * 8 - part_shift);
    auto new_length = this->length() - words_to_del;
    BigInt result = BigInt::with_cap(new_length);

    int i, j;
    for (i = words_to_del, j = 0; i < this->length(); i++, j++) {
        result.al[j] = (this->al[i] >> part_shift) | (this->al[i + 1] << hight_part_shift);
    }

    result.al[new_length - 1] = this->al[this->length() - 1] >> part_shift;
    result.ar = result.al + new_length;
    return result;
}

BigInt BigInt::operator<< (unsigned int shift) const {
    auto add_words_num = shift / (sizeof(BASE) * 8);
	auto shift_num = shift % (sizeof(BASE) * 8);
    auto new_length = this->length() + add_words_num;
	BigInt result = BigInt::with_cap(new_length);
    if (!this->length()) return result;
	BASE current_head = 0, next_head = 0;
    memset(result.al, add_words_num, 0);
	for (size_t i = add_words_num; i < new_length; i++) {
		current_head = next_head;
		next_head = this->al[i - add_words_num] >> ((sizeof(BASE) * 8) - shift_num);
		result[i] = (this->al[i - add_words_num] << shift_num) | current_head;
	}
    result.ar = result.al + new_length;
	if (next_head) {
		result.push(next_head);
	}
	return result;
}
