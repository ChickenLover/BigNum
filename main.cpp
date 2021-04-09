#include "big.h" 
#include "algos.h"


int main(int argc, char** argv){
    srand(time(NULL));
    /*BigInt a;
    std::cin >> a;
    a.print_decimal();*/
 
    // BigInt to = (BigInt(1) << 128);
    // BigInt rnd = BigInt::random_prime(BigInt((BASE)0), to);
    // std::cout << rnd << std::endl;

    // BigInt prime = "C7A942B524F86A9FAAB25CE1793B22C5D663C14F022CEB8B405F51C46E88861B";
    // std::cout << prime.is_prime() << std::endl;

    BigInt to = BigInt(1) << 128;
    BigInt rnd = BigInt::random(BigInt((BASE)0), to);
    std::cout << "Факторизуем ";
    rnd.print_decimal();
    std::cout << std::endl;
    std::vector<BigInt> primes;
    BigInt reminder = naive_division(rnd, BigInt(1) << 16, primes);
    if (reminder != 1)
        std::cout << "Факторизация неполная... Остаток: ";
        reminder.print_decimal();
        std::cout << std::endl;
    std::cout << "Простые делители: ";
    for (auto prime : primes) {
        prime.print_decimal();
        std::cout << ", ";
    }
    std::cout << std::endl;


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
