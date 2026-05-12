#ifndef LONGNUMBER_HPP
#define LONGNUMBER_HPP

#include <iostream>
#include <vector>
#include <string>

class LongNumber {
private:
    static const int BASE = 10;
    std::vector<int> digits;
    bool isNegative;
    
    void removeLeadingZeros();
    static std::vector<int> addAbsolute(const std::vector<int>& a, const std::vector<int>& b);
    static std::vector<int> subtractAbsolute(const std::vector<int>& a, const std::vector<int>& b);
    static int compareAbsolute(const std::vector<int>& a, const std::vector<int>& b);
    
public:
    // 构造函数
    LongNumber();
    LongNumber(long long num);
    LongNumber(const std::string& num);
    LongNumber(const LongNumber& other);
    LongNumber(LongNumber&& other) noexcept;
    
    // 析构函数
    ~LongNumber();
    
    // 赋值运算符
    LongNumber& operator=(const LongNumber& other);
    LongNumber& operator=(LongNumber&& other) noexcept;
    
    // 算术运算符
    LongNumber operator+(const LongNumber& other) const;
    LongNumber operator-(const LongNumber& other) const;
    LongNumber operator*(const LongNumber& other) const;
    LongNumber operator/(const LongNumber& other) const;
    LongNumber operator%(const LongNumber& other) const;
    
    // 复合赋值运算符
    LongNumber& operator+=(const LongNumber& other);
    LongNumber& operator-=(const LongNumber& other);
    LongNumber& operator*=(const LongNumber& other);
    LongNumber& operator/=(const LongNumber& other);
    LongNumber& operator%=(const LongNumber& other);
    
    // 比较运算符
    bool operator==(const LongNumber& other) const;
    bool operator!=(const LongNumber& other) const;
    bool operator<(const LongNumber& other) const;
    bool operator>(const LongNumber& other) const;
    bool operator<=(const LongNumber& other) const;
    bool operator>=(const LongNumber& other) const;
    
    // 自增/自减
    LongNumber& operator++();
    LongNumber operator++(int);
    LongNumber& operator--();
    LongNumber operator--(int);
    
    // 类型转换
    explicit operator std::string() const;
    explicit operator long long() const;
    
    // 其他成员函数
    LongNumber abs() const;
    bool isZero() const { return digits.size() == 1 && digits[0] == 0; }
    size_t getDigitCount() const { return digits.size(); }
    bool getSign() const { return isNegative; }
    
    // 友元函数声明
    friend LongNumber operator-(const LongNumber& num);
    friend std::ostream& operator<<(std::ostream& os, const LongNumber& num);
    friend std::istream& operator>>(std::istream& is, LongNumber& num);
    
    // 除法的辅助函数
    std::pair<LongNumber, LongNumber> divideWithRemainder(const LongNumber& other) const;
};

// 非成员运算符函数
LongNumber operator-(const LongNumber& num);
std::ostream& operator<<(std::ostream& os, const LongNumber& num);
std::istream& operator>>(std::istream& is, LongNumber& num);

#endif
