#pragma once

#include <ostream>
#include <string>

class BigInt {
    std::string BigIntString;
    char sign;
public:
    BigInt();
    BigInt(int number);
    BigInt(std::string number);
    BigInt(const BigInt& BigIntObject);
    ~BigInt();
    BigInt& operator=(const BigInt& BigIntObject);
    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator--();
    BigInt operator--(int);
    BigInt& operator+=(const BigInt& BigIntObject);
    BigInt& operator*=(const BigInt& BigIntObject);
    BigInt& operator-=(const BigInt& BigIntObject);
    BigInt& operator/=(const BigInt& BigIntObject);
    BigInt& operator^=(const BigInt& BigIntObject);
    BigInt& operator%=(const BigInt& BigIntObject);
    BigInt& operator&=(const BigInt& BigIntObject);
    BigInt& operator|=(const BigInt& BigIntObject);
    BigInt operator~() const;
    BigInt operator+() const;
    BigInt operator-() const;
    bool operator==(const BigInt& BigIntObject) const;
    bool operator!=(const BigInt& BigIntObject) const;
    bool operator<(const BigInt& BigIntObject) const;
    bool operator>(const BigInt& BigIntObject) const;
    bool operator<=(const BigInt& BigIntObject) const;
    bool operator>=(const BigInt& BigIntObject) const;
    operator int() const;
    operator std::string() const;
    size_t size() const;
};
BigInt operator+(const BigInt& BigIntFirst, const BigInt& BigIntSecond);
BigInt operator-(const BigInt& BigIntFirst, const BigInt& BigIntSecond);
BigInt operator*(const BigInt& BigIntFirst, const BigInt& BigIntSecond);
BigInt operator/(const BigInt& BigIntFirst, const BigInt& BigIntSecond);
BigInt operator^(const BigInt& BigIntFirst, const BigInt& BigIntSecond);
BigInt operator%(const BigInt& BigIntFirst, const BigInt& BigIntSecond);
BigInt operator&(const BigInt& BigIntFirst, const BigInt& BigIntSecond);
BigInt operator|(const BigInt& BigIntFirst, const BigInt& BigIntSecond);
std::ostream& operator<<(std::ostream& o, BigInt& i);

