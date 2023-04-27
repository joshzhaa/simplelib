#include <vector>
#include <string>
#include <cassert>

template<typename T>
concept Integer = std::is_integral<T>::value;

template<typename T>
concept Signed = Integer<T> && std::is_signed<T>::value;

template<typename T>
concept Unsigned = Integer<T> && !Signed<T>;

template<typename T>
concept Arithmetic = std::is_arithmetic<T>::value;

// represents a number with an arbitrarily large number of digits (but not infinite)
// invariant: on function exit, Number must be in simplified form (not leading or trailing zeroes)
class Number {
    std::vector<bool> radix_left; // first immediately left of point, second, third, ...
    std::vector<bool> radix_right;
    bool positive; // internally 0 must be positive, but this isn't viewable by the user
    
    // @@@@ helpers for converting between Number and other types - type.cpp
    template<Integer T>
    void unsigned_helper(T num) {
        while (num) {
            bool least_significant = num & 1;
            radix_left.push_back(least_significant);
            num >>= 1;
        }
    }
    template<Arithmetic T>
    [[nodiscard]] T negative_helper(T num) {
        if (num >= 0) return num;
        positive = false;
        return num * -1;
    }
    bool is_zero() const { return radix_left.empty() && radix_right.empty(); }
    void simplify(); // drop leading and trailing zeroes

    // @@@@ helpers for math operations - math.cpp
    static Number add_unsigned(const Number& A, const Number& B);
    bool less_unsigned(const Number& other) const;
    // retrieve the bit at index of both bool vectors, returns tuple (bit1, bit2)
    static std::tuple<bool, bool> get_bits(size_t index, const std::vector<bool>&, const std::vector<bool>&);

public:
    Number() : radix_left(), radix_right(), positive(true) {}
    template<Arithmetic T>
    Number(T num) : Number() { *this = num; }

    // @@@@ math operations - math.cpp
    Number operator+(Number other) const;
    Number operator~() const;
    bool operator==(const Number& other) const;
    bool operator<(const Number& other) const;
    
    // @@@@ set value of Number by a built in numeric type - type.cpp
    template<Unsigned T>
    void operator=(T num) {
        *this = Number(); // reset to zero
        unsigned_helper(num);
    }
    template<Signed T>
    void operator=(T num) {
        *this = Number();
        num = negative_helper(num);    
        unsigned_helper(num);
    }
    void operator=(float num);
    void operator=(double num);

    // @@@@ convert Number to other data types - type.cpp
    std::string to_string() const;
    operator bool() const;
    operator unsigned() const;
    operator int() const;
};
