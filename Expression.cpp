#include "Expression.hpp"

std::ostream& operator<<(std::ostream& out, const Expression& expr) {
    expr.print(out);
    return out;
}

UnaryExpression::UnaryExpression(const Expression& op) : operand(op.clone()) {}

UnaryExpression::UnaryExpression(const UnaryExpression& other) : operand(other.operand->clone()) {}

UnaryExpression::UnaryExpression(UnaryExpression&& other) noexcept : operand(other.operand) {
    other.operand = nullptr;
}

UnaryExpression& UnaryExpression::operator=(const UnaryExpression& other) {
    if (this != &other) {
        delete operand;
        operand = other.operand->clone();
    }
    return *this;
}

UnaryExpression& UnaryExpression::operator=(UnaryExpression&& other) noexcept {
    if (this != &other) {
        delete operand;
        operand = other.operand;
        other.operand = nullptr;
    }
    return *this;
}

UnaryExpression::~UnaryExpression() {
    delete operand;
}

BinaryExpression::BinaryExpression(const Expression& l, const Expression& r) 
    : left(l.clone()), right(r.clone()) {}

BinaryExpression::BinaryExpression(const BinaryExpression& other) 
    : left(other.left->clone()), right(other.right->clone()) {}

BinaryExpression::BinaryExpression(BinaryExpression&& other) noexcept 
    : left(other.left), right(other.right) {
    other.left = nullptr;
    other.right = nullptr;
}

BinaryExpression& BinaryExpression::operator=(const BinaryExpression& other) {
    if (this != &other) {
        delete left;
        delete right;
        left = other.left->clone();
        right = other.right->clone();
    }
    return *this;
}

BinaryExpression& BinaryExpression::operator=(BinaryExpression&& other) noexcept {
    if (this != &other) {
        delete left;
        delete right;
        left = other.left;
        right = other.right;
        other.left = nullptr;
        other.right = nullptr;
    }
    return *this;
}

BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}

Const::Const(Int128 val) : value(val) {}
Const::Const(int64_t val) : value(Int128(val)) {}

Int128 Const::eval(const std::unordered_map<std::string, Int128>& /*vars*/) const {
    return value;
}

Expression* Const::clone() const {
    return new Const(value);
}

void Const::print(std::ostream& out) const {
    out << value;
}


Variable::Variable(std::string n) : name(std::move(n)) {}

Int128 Variable::eval(const std::unordered_map<std::string, Int128>& vars) const {
    return vars.at(name); 
}

Expression* Variable::clone() const {
    return new Variable(name);
}

void Variable::print(std::ostream& out) const {
    out << name;
}

Negate::Negate(const Expression& op) : UnaryExpression(op) {}
Int128 Negate::eval(const std::unordered_map<std::string, Int128>& vars) const {
    return -operand->eval(vars);
}
Expression* Negate::clone() const { return new Negate(*operand); }
void Negate::print(std::ostream& out) const { out << "(-" << *operand << ")"; }

Add::Add(const Expression& l, const Expression& r) : BinaryExpression(l, r) {}
Int128 Add::eval(const std::unordered_map<std::string, Int128>& vars) const {
    return left->eval(vars) + right->eval(vars);
}
Expression* Add::clone() const { return new Add(*left, *right); }
void Add::print(std::ostream& out) const { out << "(" << *left << " + " << *right << ")"; }

Subtract::Subtract(const Expression& l, const Expression& r) : BinaryExpression(l, r) {}
Int128 Subtract::eval(const std::unordered_map<std::string, Int128>& vars) const {
    return left->eval(vars) - right->eval(vars);
}
Expression* Subtract::clone() const { return new Subtract(*left, *right); }
void Subtract::print(std::ostream& out) const { out << "(" << *left << " - " << *right << ")"; }

Multiply::Multiply(const Expression& l, const Expression& r) : BinaryExpression(l, r) {}
Int128 Multiply::eval(const std::unordered_map<std::string, Int128>& vars) const {
    return left->eval(vars) * right->eval(vars);
}
Expression* Multiply::clone() const { return new Multiply(*left, *right); }
void Multiply::print(std::ostream& out) const { out << "(" << *left << " * " << *right << ")"; }

Divide::Divide(const Expression& l, const Expression& r) : BinaryExpression(l, r) {}
Int128 Divide::eval(const std::unordered_map<std::string, Int128>& vars) const {
    return left->eval(vars) / right->eval(vars);
}
Expression* Divide::clone() const { return new Divide(*left, *right); }
void Divide::print(std::ostream& out) const { out << "(" << *left << " / " << *right << ")"; }

Negate operator-(const Expression& op) { return Negate(op); }
Add operator+(const Expression& lhs, const Expression& rhs) { return Add(lhs, rhs); }
Subtract operator-(const Expression& lhs, const Expression& rhs) { return Subtract(lhs, rhs); }
Multiply operator*(const Expression& lhs, const Expression& rhs) { return Multiply(lhs, rhs); }
Divide operator/(const Expression& lhs, const Expression& rhs) { return Divide(lhs, rhs); }