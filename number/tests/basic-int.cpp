#include "number.h"
#include <iostream>

int main() {
    Number sum;
    unsigned counter = 0;
    for (Number i = Number(-100); i < Number(100); i = i + Number(1)) {
        std::cout << (int) sum << " (" << sum.to_string() << ") + " 
            << (int) i << " (" << i.to_string() << ") = " 
            << (int)(sum + i) << " (" << (sum + i).to_string() << ")\n";
        sum = sum + i;
        if (counter++ > 10) break;
    }
    return 0;
}
