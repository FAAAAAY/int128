#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include "int128.hpp"

class Expression {
public:
    virtual ~Expression() = default;

    virtual Int128 eval(const std::unordered_map<std::string, Int128>& vars) const = 0;
    virtual Expression* clone() const = 0;
    virtual void print(std::ostream& out) const = 0;

    friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

// ================= Базовые классы для управления памятью =================

class UnaryExpression : public Expression {
protected:
    Expression* operand;
public:
    explicit UnaryExpression(const Expression& op);
    UnaryExpression(const UnaryExpression& other);
    UnaryExpression(UnaryExpression&& other) noexcept;
    UnaryExpression& operator=(const UnaryExpression& other);
    UnaryExpression& operator=(UnaryExpression&& other) noexcept;
    ~UnaryExpression() override;
};

class BinaryExpression : public Expression {
protected:
    Expression* left;
    Expression* right;
public:
    BinaryExpression(const Expression& l, const Expression& r);
    BinaryExpression(const BinaryExpression& other);
    BinaryExpression(BinaryExpression&& other) noexcept;
    BinaryExpression& operator=(const BinaryExpression& other);
    BinaryExpression& operator=(BinaryExpression&& other) noexcept;
    ~BinaryExpression() override;
};

// ================= Листья дерева =================

class Const : public Expression {
    Int128 value;
public:
    explicit Const(Int128 val);
    explicit Const(int64_t val); // Для удобства Const(2)

    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& out) const override;
};

class Variable : public Expression {
    std::string name;
public:
    explicit Variable(std::string n);

    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& out) const override;
};

// ================= Операции =================

class Negate : public UnaryExpression {
public:
    explicit Negate(const Expression& op);
    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& out) const override;
};

class Add : public BinaryExpression {
public:
    Add(const Expression& l, const Expression& r);
    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& out) const override;
};

class Subtract : public BinaryExpression {
public:
    Subtract(const Expression& l, const Expression& r);
    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& out) const override;
};

class Multiply : public BinaryExpression {
public:
    Multiply(const Expression& l, const Expression& r);
    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& out) const override;
};

class Divide : public BinaryExpression {
public:
    Divide(const Expression& l, const Expression& r);
    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& out) const override;
};

// ================= Перегрузка операторов (Syntactic Sugar) =================

Negate operator-(const Expression& op);
Add operator+(const Expression& lhs, const Expression& rhs);
Subtract operator-(const Expression& lhs, const Expression& rhs);
Multiply operator*(const Expression& lhs, const Expression& rhs);
Divide operator/(const Expression& lhs, const Expression& rhs);