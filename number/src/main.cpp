#include "number.h"
#include <iostream>

std::ostream& operator<<(std::ostream& out, const Number& num) {
    return out << num.to_string();
}

int main() {
    Number A(-100);
    Number B(0);
    std::cout << (int) A << " (" << A.to_string() << ") + " 
        << (int) B << " (" << B.to_string() << ") = " 
        << (int)(A + B) << " (" << (A + B).to_string() << ")\n";
    return 0;
}
