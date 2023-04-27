#include "number.h"
#include <iostream>

void Number::simplify() {
    auto drop_zeroes = [](std::vector<bool>& bits) {
        auto it = bits.rbegin();
        while (it != bits.rend() && !(*it)) ++it;
        bits.erase(it.base(), bits.end());
    };
    drop_zeroes(radix_right);
    drop_zeroes(radix_left);
}

void Number::operator=(float num) {
    *this = Number();
    num = negative_helper(num);
    constexpr unsigned MANTISSA_SIZE = 23;
    constexpr unsigned MANTISSA_MASK = ~(0b111111111 << MANTISSA_SIZE);
    uint32_t* ptr = reinterpret_cast<uint32_t*>(&num); // necessary to get bits of float
    unsigned exponent = (*ptr >> MANTISSA_SIZE) - 127;
    unsigned mantissa = *ptr & MANTISSA_MASK;
    
    // fill digits right of point
    unsigned bit_mask = 1 << (MANTISSA_SIZE - exponent - 1);
    do {
        radix_right.push_back(mantissa & bit_mask);
        bit_mask >>= 1;
    } while (bit_mask);
    // fill digits left of point
    bit_mask = 1 << (MANTISSA_SIZE - exponent);
    do {
        radix_left.push_back(mantissa & bit_mask);
        bit_mask <<= 1;
    } while (bit_mask != 1 << MANTISSA_SIZE);
    radix_left.push_back(1);

    simplify();
}

void Number::operator=(double) {
    assert(false); // TODO: unimplemented
}

std::string Number::to_string() const {
    // edge case, if Number == 0 the general algo would print nothing, this is confusing
    if (radix_left.empty() && radix_right.empty()) return "0";
    std::string str = positive ? "" : "-";
    auto append = [&](bool bit) {
        str.push_back(bit ? '1' : '0');
    };
    for (auto it = radix_left.rbegin(); it != radix_left.rend(); ++it) append(*it);
    if (!radix_right.empty()) str.push_back('.');
    for (auto bit : radix_right) append(bit);
    return str;
}
    
Number::operator bool() const {
    return !is_zero();
}

Number::operator unsigned() const {
    unsigned result = 0;
    for (unsigned i = 0; i < 8 * sizeof(unsigned); ++i) {
        bool bit = i < radix_left.size() ? radix_left[i] : 0;
        if (bit) result |= 1 << i;
    }
    return result;
}

Number::operator int() const {
    int result = 0;
    for (unsigned i = 0; i < 8 * sizeof(int) - 1; ++i) {
        bool bit = i < radix_left.size() ? radix_left[i] : 0;
        if (bit) result |= 1 << i;
    }
    if (!positive) result *= -1;
    return result;
}
