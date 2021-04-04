#include "big.h"

bool BigInt::operator< (const BigInt &other) const{
    if(this->length() != other.length())
        return this->length() < other.length();
    for(int i=this->length()-1; i>0; i--){
        if((*this)[i] != other[i])
            return (*this)[i] < other[i];
    }
    return false; // If equal
}

bool BigInt::operator> (const BigInt &other) const{
    return !((*this) <= other);
}

bool BigInt::operator<= (const BigInt &other) const{
    return (*this) < other || (*this) == other;
}

bool BigInt::operator>= (const BigInt &other) const{
    return (*this) > other || (*this) == other;
}

bool BigInt::operator== (const BigInt &other) const{
    if(this->length() != other.length())
        return false;
    for(int i=0; i<this->length(); i++){
        if((*this)[i] != other[i])
            return false;
    }
    return true;
}

bool BigInt::operator!= (const BigInt &other) const{
    return !((*this) == other );
}

bool BigInt::operator< (const BASE &other) const {
    return this->length() == 0 || (this->length() == 1 && (*this)[0] < other);
}

bool BigInt::operator> (const BASE &other) const {
    return !((*this) <= other);
}

bool BigInt::operator>= (const BASE &other) const {
    return (*this) > other || (*this) == other;
}

bool BigInt::operator<= (const BASE &other) const {
    return (*this) < other || (*this) == other;
}

bool BigInt::operator== (const BASE &other) const {
    return this->length() == 1 && (*this)[0] == other;
}

bool BigInt::operator!= (const BASE &other) const{
    return !((*this) == other );
}
