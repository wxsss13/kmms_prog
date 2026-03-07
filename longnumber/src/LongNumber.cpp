#include "Longnumber.hpp"
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <iostream>

// 修复1: 将循环变量类型改为 size_t
void LongNumber::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.size() == 1 && digits[0] == 0) {
        isNegative = false;
    }
}

std::vector<int> LongNumber::addAbsolute(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    int carry = 0;
    size_t maxSize = std::max(a.size(), b.size());
    
    for (size_t i = 0; i < maxSize || carry; ++i) {
        if (i < a.size()) carry += a[i];
        if (i < b.size()) carry += b[i];
        result.push_back(carry % BASE);
        carry /= BASE;
    }
    return result;
}

std::vector<int> LongNumber::subtractAbsolute(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    int borrow = 0;
    
    for (size_t i = 0; i < a.size(); ++i) {
        int digitA = a[i];
        int digitB = (i < b.size()) ? b[i] : 0;
        
        digitA -= borrow;
        if (digitA < digitB) {
            digitA += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.push_back(digitA - digitB);
    }
    
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    return result;
}

int LongNumber::compareAbsolute(const std::vector<int>& a, const std::vector<int>& b) {
    if (a.size() != b.size()) {
        return a.size() < b.size() ? -1 : 1;
    }
    // 修复2: 使用 size_t 类型
    for (size_t i = a.size(); i > 0; --i) {
        if (a[i-1] != b[i-1]) {
            return a[i-1] < b[i-1] ? -1 : 1;
        }
    }
    return 0;
}

LongNumber::LongNumber() : digits(1, 0), isNegative(false) {}

LongNumber::LongNumber(long long num) {
    isNegative = num < 0;
    long long absNum = isNegative ? -num : num;
    
    if (absNum == 0) {
        digits.push_back(0);
    } else {
        while (absNum > 0) {
            digits.push_back(static_cast<int>(absNum % BASE));
            absNum /= BASE;
        }
    }
}

LongNumber::LongNumber(const std::string& num) {
    if (num.empty()) {
        digits.push_back(0);
        isNegative = false;
        return;
    }
    
    size_t start = 0;
    isNegative = (num[0] == '-');
    if (num[0] == '+' || num[0] == '-') {
        start = 1;
    }
    
    // 修复3: 使用正确的循环方式
    for (size_t i = num.length(); i > start; --i) {
        if (!std::isdigit(num[i-1])) {
            throw std::invalid_argument("Invalid character in number string");
        }
        digits.push_back(num[i-1] - '0');
    }
    
    removeLeadingZeros();
}

LongNumber::LongNumber(const LongNumber& other) 
    : digits(other.digits), isNegative(other.isNegative) {
    std::cout << "拷贝构造函数调用" << std::endl;
}

LongNumber::LongNumber(LongNumber&& other) noexcept
    : digits(std::move(other.digits)), isNegative(other.isNegative) {
    std::cout << "移动构造函数调用" << std::endl;
    other.digits.clear();
    other.digits.push_back(0);
    other.isNegative = false;
}

LongNumber::~LongNumber() {
    std::cout << "析构函数调用" << std::endl;
}

LongNumber& LongNumber::operator=(const LongNumber& other) {
    std::cout << "拷贝赋值运算符调用" << std::endl;
    if (this != &other) {
        digits = other.digits;
        isNegative = other.isNegative;
    }
    return *this;
}

LongNumber& LongNumber::operator=(LongNumber&& other) noexcept {
    std::cout << "移动赋值运算符调用" << std::endl;
    if (this != &other) {
        digits = std::move(other.digits);
        isNegative = other.isNegative;
        
        other.digits.clear();
        other.digits.push_back(0);
        other.isNegative = false;
    }
    return *this;
}

LongNumber LongNumber::operator+(const LongNumber& other) const {
    if (isNegative == other.isNegative) {
        LongNumber result;
        result.digits = addAbsolute(digits, other.digits);
        result.isNegative = isNegative;
        result.removeLeadingZeros();
        return result;
    } else {
        int cmp = compareAbsolute(digits, other.digits);
        if (cmp == 0) {
            return LongNumber(0);
        }
        
        LongNumber result;
        if (cmp > 0) {
            result.digits = subtractAbsolute(digits, other.digits);
            result.isNegative = isNegative;
        } else {
            result.digits = subtractAbsolute(other.digits, digits);
            result.isNegative = other.isNegative;
        }
        result.removeLeadingZeros();
        return result;
    }
}

LongNumber LongNumber::operator-(const LongNumber& other) const {
    return *this + (-other);
}

LongNumber LongNumber::operator*(const LongNumber& other) const {
    if (isZero() || other.isZero()) {
        return LongNumber(0);
    }
    
    LongNumber result;
    result.digits.resize(digits.size() + other.digits.size(), 0);
    
    for (size_t i = 0; i < digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            long long current = result.digits[i + j] + 
                               digits[i] * (j < other.digits.size() ? other.digits[j] : 0) + 
                               carry;
            result.digits[i + j] = static_cast<int>(current % BASE);
            carry = static_cast<int>(current / BASE);
        }
    }
    
    result.isNegative = isNegative != other.isNegative;
    result.removeLeadingZeros();
    return result;
}

// 修复4: 修复 divideWithRemainder 函数中的私有成员访问问题
std::pair<LongNumber, LongNumber> LongNumber::divideWithRemainder(const LongNumber& other) const {
    if (other.isZero()) {
        throw std::runtime_error("Division by zero");
    }
    
    LongNumber dividend = abs();
    LongNumber divisor = other.abs();
    
    if (dividend < divisor) {
        return {LongNumber(0), *this};
    }
    
    LongNumber quotient;
    LongNumber remainder;
    quotient.digits.resize(dividend.digits.size(), 0);
    
    // 修复: 使用 size_t 类型
    for (size_t i = dividend.digits.size(); i > 0; --i) {
        remainder.digits.insert(remainder.digits.begin(), dividend.digits[i-1]);
        remainder.removeLeadingZeros();
        
        int qDigit = 0;
        int low = 0, high = BASE - 1;
        
        while (low <= high) {
            int mid = (low + high) / 2;
            LongNumber product = divisor * LongNumber(mid);
            
            if (product <= remainder) {
                qDigit = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        
        quotient.digits[i-1] = qDigit;
        remainder = remainder - divisor * LongNumber(qDigit);
    }
    
    quotient.removeLeadingZeros();
    
    // 修复: 使用 getSign() 而不是直接访问私有成员
    quotient.isNegative = (isNegative != other.isNegative);
    remainder.isNegative = isNegative;  // 这里使用当前对象的 isNegative
    
    return {quotient, remainder};
}

LongNumber LongNumber::operator/(const LongNumber& other) const {
    return divideWithRemainder(other).first;
}

LongNumber LongNumber::operator%(const LongNumber& other) const {
    return divideWithRemainder(other).second;
}

LongNumber& LongNumber::operator+=(const LongNumber& other) {
    *this = *this + other;
    return *this;
}

LongNumber& LongNumber::operator-=(const LongNumber& other) {
    *this = *this - other;
    return *this;
}

LongNumber& LongNumber::operator*=(const LongNumber& other) {
    *this = *this * other;
    return *this;
}

LongNumber& LongNumber::operator/=(const LongNumber& other) {
    *this = *this / other;
    return *this;
}

LongNumber& LongNumber::operator%=(const LongNumber& other) {
    *this = *this % other;
    return *this;
}

bool LongNumber::operator==(const LongNumber& other) const {
    return isNegative == other.isNegative && digits == other.digits;
}

bool LongNumber::operator!=(const LongNumber& other) const {
    return !(*this == other);
}

bool LongNumber::operator<(const LongNumber& other) const {
    if (isNegative != other.isNegative) {
        return isNegative;
    }
    if (isNegative) {
        return abs() > other.abs();
    }
    int cmp = compareAbsolute(digits, other.digits);
    return cmp < 0;
}

bool LongNumber::operator>(const LongNumber& other) const {
    return other < *this;
}

bool LongNumber::operator<=(const LongNumber& other) const {
    return !(other < *this);
}

bool LongNumber::operator>=(const LongNumber& other) const {
    return !(*this < other);
}

LongNumber& LongNumber::operator++() {
    *this = *this + LongNumber(1);
    return *this;
}

LongNumber LongNumber::operator++(int) {
    LongNumber temp = *this;
    ++(*this);
    return temp;
}

LongNumber& LongNumber::operator--() {
    *this = *this - LongNumber(1);
    return *this;
}

LongNumber LongNumber::operator--(int) {
    LongNumber temp = *this;
    --(*this);
    return temp;
}

LongNumber::operator std::string() const {
    if (isZero()) return "0";
    
    std::string result;
    if (isNegative) result += '-';
    
    // 修复: 使用 size_t 类型
    for (size_t i = digits.size(); i > 0; --i) {
        result += std::to_string(digits[i-1]);
    }
    
    return result;
}

LongNumber::operator long long() const {
    long long result = 0;
    // 修复: 使用 size_t 类型
    for (size_t i = digits.size(); i > 0; --i) {
        result = result * BASE + digits[i-1];
        if (result < 0) {
            throw std::overflow_error("Number too large for long long");
        }
    }
    return isNegative ? -result : result;
}

LongNumber LongNumber::abs() const {
    LongNumber result(*this);
    result.isNegative = false;
    return result;
}

LongNumber operator-(const LongNumber& num) {
    LongNumber result(num);
    if (!num.isZero()) {
        result.isNegative = !num.isNegative;
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const LongNumber& num) {
    if (num.isNegative) os << '-';
    // 修复: 使用 size_t 类型
    for (size_t i = num.digits.size(); i > 0; --i) {
        os << num.digits[i-1];
    }
    return os;
}

std::istream& operator>>(std::istream& is, LongNumber& num) {
    std::string s;
    is >> s;
    num = LongNumber(s);
    return is;
}
