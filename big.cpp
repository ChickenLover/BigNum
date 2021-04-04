#include "big.h" 
BigInt::BigInt(){}

BigInt::BigInt(const char *chars){
    this->from_hex(chars);
}

BigInt::BigInt(const BigInt &other){
    this->al = new BASE[other.capacity()];
    for(size_t i=0; i<other.length(); i++){
        this->al[i] = other.al[i];
    }
    this->ar = this->al + other.length();
    this->ah = this->al + other.capacity();
}

BigInt::BigInt(const BASE val) : BigInt() {
    this->al = new BASE[1];
    this->ar = this->al + 1;
    this->ah = this->al + 1;
    this->al[0] = val;
}

BigInt::~BigInt(){
    delete [] al;
}

void BigInt::from_hex(const char *hex){
    int hex_len = strlen(hex);
    int base_len = ceil((float)ceil((float)hex_len / 2) / sizeof(BASE));
    while(this->capacity() < base_len){
        this->extend_alloc();
    }
    char hex_per_base = sizeof(BASE) * 2;
    int hex_multiplier; // 8 * 10^0, 8 * 10^1 ... 8 * 10^n = hex_multiplier
    for(int i=hex_len; i>0; i--){
        int invert_index = hex_len - i;
        int base_index = invert_index / hex_per_base;
        if(!(invert_index % hex_per_base)){
            this->al[base_index] = 0;
            this->ar++;
            hex_multiplier = 1;
        }
        this->al[base_index] += hex_to_num(hex[i - 1]) * hex_multiplier;
        hex_multiplier = hex_multiplier * 16;
    }
}

BigInt BigInt::with_cap(size_t capacity) {
    BigInt ret;
    ret.al = new BASE[capacity];
    ret.ar = ret.al;
    ret.ah = ret.al + capacity;
    return ret;
}

BigInt BigInt::random(BASE chunks) {
    BigInt from = BigInt::with_cap(chunks);
    BigInt to;
	for (size_t i = 0; i < chunks - 1; i++) {
		to.push(-1);
	}
    return BigInt::random(from, to);
}

BigInt BigInt::random(BigInt from, BigInt to) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<BASE> distr;
	BigInt bound = to - from;
	BigInt max, unbound;
	for (size_t i = 0; i < bound.length(); i++) {
		max.push(-1);
		unbound.push(distr(gen));
	}
    BigInt divider = max / (bound + 1) + 1;
	return from + unbound / divider;
}

BigInt BigInt::random_prime(BigInt from, BigInt to) {
    BigInt result = (BASE)0;
    while (!result.is_prime()) {
        result = BigInt::random(from, to);
    }
    return result;
}

void BigInt::extend_alloc(){
    BASE *new_base = new BASE[(this->capacity() + 1) * EXTENSION_MUL];
    for(size_t i=0; i<this->length(); i++){
        new_base[i] = this->al[i];
    }
    this->ar = new_base + this->length();
    this->ah = new_base + (this->capacity() + 1) * EXTENSION_MUL;
    if(this->al)
        delete [] this->al;
    this->al = new_base;
}

size_t BigInt::length() const{
    return this->ar - this->al;
}

size_t BigInt::capacity() const{
    return this->ah - this->al;
}

BASE &BigInt::operator[] (const size_t index){
    return this->al[index];
}

BASE BigInt::operator[] (const size_t index) const{
    return this->al[index];
}

BigInt &BigInt::operator= (const BigInt &other){
    if(this == &other)
        return *this;
    if(this->capacity() < other.length()){
        delete [] this->al;
        this->al = new BASE[other.capacity()];
    }
    for(size_t i=0; i<other.length(); i++){
        this->al[i] = other.al[i];
    }
    this->ar = this->al + other.length();
    this->ah = this->al + other.capacity();
    return *this;
}

BigInt &BigInt::operator= (BASE other) {
    if (this->capacity())
        delete [] this->al;
    this->al = new BASE[1];
    this->al[0] = other;
    this->ar = this->al + 1;
    this->ah = this->ar;
}

std::ostream &operator<< (std::ostream &os, const BigInt &number){
    if(!number.length() || (number.length() == 1 && !number[0])){
        os << "0";
        return os;
    }
    int hex_per_base = sizeof(BASE) * 2;
    bool zeroes_stripped = false;
    for(int i=number.length(); i>0; i--){
        BASE next_element = number[i - 1];
        int hex_multiplier = pow(16, hex_per_base - 1);
        for(int j=0; j<hex_per_base; j++){
            char symbol = num_to_hex(next_element / hex_multiplier);
            next_element %= hex_multiplier;
            hex_multiplier /= 16;
            if(!zeroes_stripped && symbol != '0'){
                zeroes_stripped = true;
            }
            if(!zeroes_stripped)
                continue;
            os << symbol;
        }
        //os << "|";
    }
    if(!zeroes_stripped)
        os << "0";
	return os;
}

void BigInt::print_decimal(){
    if(!this->length() || (this->length() == 1 && !(*this)[0])){
        std::cout << "0";
        return;
    }
    std::string representation;
    BigInt copy = (*this);
    while(copy.length()){
        representation.push_back(copy % 10);
        copy = copy / 10;
    }
    for(auto it = representation.rbegin(); it != representation.rend(); ++it) {
        std::cout << (int)*it;
    }
}

std::istream &operator>> (std::istream &is, BigInt &number){
    std::string hex;
    is >> hex;
    number.from_hex(hex.c_str());
	return is;
}

void BigInt::push(const BASE new_el){
    if(this->ar == this->ah)
        this->extend_alloc();
    *(this->ar) = new_el;
    this->ar++;
}

BASE BigInt::pop(){
    return *(--this->ar);
}

void BigInt::zero(){
    this->ar = this->al;
}

void BigInt::lstrip(){
    for(int i=this->length()-1; i>=0 && !(*this)[i]; i--){
        this->pop();
    }
}

int main(int argc, char** argv){
    srand(time(NULL));
    /*BigInt a;
    std::cin >> a;
    a.print_decimal();*/

   
    BigInt rnd = BigInt::random_prime(BigInt((BASE)0), BigInt(1) << 1024);
    std::cout << rnd << std::endl;
    

    //POWER TESTS
    //BigInt a = argv[1];
    //BASE n = atoi(argv[2]);
    //BigInt modulus = argv[3];
    //std::cout << a.pow(n, modulus) << std::endl;


    //RECOMBINITION TESTS
    //BigInt a = argv[1];
    //BigInt b = argv[2];
    //BigInt div = a / b;
    //std::cout << div << std::endl;
    //BigInt div = a / b;
    //BigInt reminder = a % b;
    //BigInt recombined = b * div + reminder;
    //std::cout << recombined << std::endl;

    /*
    //GRIBANOV DIVISION TESTS
    BigInt a;
    BigInt b;
    std::cin >> a;
    std::cin >> b;
    std::cout << a / b << std::endl << a % b;
    */


    /*
    // ADDING TESTS
    BigInt a, b;
    std::cin >> a;
    std::cin >> b;
    BigInt sum = a + b;
    std::cout << sum;
    */

    /* // SUBSTRUCTION TESTS
    BigInt sub = a - b;
    std::cout << sub;
    */

    /* // MULTIPLICATION TESTS
    BigInt mul = a * b;
    std::cout << mul;
    */

    /*
    // DIVISION TESTS
    BigInt a;
    std::cin >> a;
    std::string hex_val;
    std::cin >> hex_val;
    BASE c = hex_to_base(hex_val.c_str());
    BigInt div = a / c;
    BASE reminder = a % c;
    std::cout << div << std::endl;
    base_hex(std::cout, reminder);
    */

    /*
    //SUPER DIVISION TESTS
    BigInt a = argv[1];
    BigInt b = argv[2];
    BigInt div = a / b;
    std::cout << div << std::endl;
    BigInt reminder = a % b;
    std::cout << reminder << std::endl;
    */
    return 0;
}
