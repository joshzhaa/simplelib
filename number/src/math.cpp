#include "number.h"
#include <tuple>

// bool operators
#define XOR !=
#define AND &&
#define NOT !
#define OR ||

Number Number::add_unsigned(const Number& left, const Number& right) {
    Number result;
    result.radix_left.resize(std::max(left.radix_left.size(), right.radix_left.size()));
    result.radix_right.resize(std::max(left.radix_right.size(), right.radix_right.size()));
    // full adder returns tuple (sum, carry)
    auto add = [](bool A, bool B, bool carry_in) {
        bool sum = A XOR B XOR carry_in;
        bool carry_out = (A AND B) OR (carry_in AND (A XOR B));
        return std::make_tuple(sum, carry_out);
    };
    bool carry_bit = 0;
    for (size_t i = result.radix_right.size(); i > 0; --i) {
        size_t idx = i - 1;
        auto [bit1, bit2] = get_bits(idx, left.radix_right, right.radix_right);
        auto [sum, carry_out] = add(bit1, bit2, carry_bit);
        result.radix_right[idx] = sum;
        carry_bit = carry_out;
    }
    for (size_t idx = 0; idx < result.radix_left.size(); ++idx) {
        auto [bit1, bit2] = get_bits(idx, left.radix_left, right.radix_left);
        auto [sum, carry_out] = add(bit1, bit2, carry_bit);
        result.radix_left[idx] = sum;
        carry_bit = carry_out;
    }
    return result;
}

std::tuple<bool, bool> Number::get_bits(size_t index, const std::vector<bool>& left, const std::vector<bool>& right) {
    bool bit1 = index < left.size() ? left[index] : 0;
    bool bit2 = index < right.size() ? right[index] : 0;
    return std::make_tuple(bit1, bit2);
};

// constructs adder-subtractor from add_unsigned adder, based on 2's comp for fixed width int
Number Number::operator+(Number other) const {
    Number self(*this);
    // calculate the sign of the result
    bool result_positive;
    if (self.positive XOR other.positive) {
        result_positive = other.less_unsigned(self);
        if (other.positive) result_positive = !result_positive;
    } else {
        assert(self.positive == other.positive);
        result_positive = self.positive;
    }
    // calculates the +1 Number as it would be in 2's comp adder-subtractor
    auto calculate_smallest = [](const std::vector<bool>& bits) {
        Number smallest;
        smallest.radix_right.resize(bits.size());
        if (!bits.empty()) smallest.radix_right.back() = 1;
        return smallest;
    };
    // negate exactly one addend if necessary
    constexpr char THIS_POSITIVE = 0b10;
    constexpr char OTHER_POSITIVE = 0b01;
    bool has_extraneous_bit = true; // in the subtraction case, must drop an extraneous 1 at the end
    switch (self.positive << 1 | other.positive) {
        case THIS_POSITIVE:
            other = add_unsigned(~other, calculate_smallest(other.radix_right));
            break;
        case OTHER_POSITIVE:
            self = add_unsigned(~self, calculate_smallest(self.radix_right));
            break;
        default: // when both have same sign, the result is positive, no negation
            has_extraneous_bit = false;
    }
    Number result = add_unsigned(self, other);
    //if (has_extraneous_bit) result.radix_left.pop_back(); // if subtraction, result has extraneous 1
    result.positive = result_positive;
    result.simplify();
    return result;
}

Number Number::operator~() const {
    Number negated(*this);
    auto negate_all = [](std::vector<bool>& bits) {
        for (auto it = bits.begin(); it != bits.end(); ++it) {
            *it = NOT *it;
        }
    };
    negate_all(negated.radix_right);
    negate_all(negated.radix_left);
    return negated;
}

bool Number::operator==(const Number& other) const {
    return positive == other.positive
        && radix_left == other.radix_left
        && radix_right == other.radix_right;
}

bool Number::less_unsigned(const Number& other) const {
    // look at the number of bits left of radix point
    if (radix_left.size() < other.radix_left.size()) return true;
    if (radix_left.size() != other.radix_left.size()) return false;
    // find the first different bit
    for (size_t i = radix_left.size(); i > 0; --i) {
        const auto [left_bit, right_bit] = get_bits(i - 1, radix_left, other.radix_left);
        if (left_bit != right_bit) return left_bit < right_bit;
    } // at this point *this and other are equal left of the radix point
    for (size_t i = 0; i < radix_right.size(); ++i) {
        const auto [left_bit, right_bit] = get_bits(i, radix_right, other.radix_right);
        if (left_bit != right_bit) return left_bit < right_bit;
    }
    return false;
}

bool Number::operator<(const Number& other) const {
    if (this->positive == other.positive) {
        return less_unsigned(other);
    } else {
        return other.positive;
    }
}
