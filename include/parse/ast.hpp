#ifndef CBB_PARSE_AST_HPP
#define CBB_PARSE_AST_HPP

#include <iostream>

#include "core/types.hpp"
#include "core/span.hpp"
#include "core/arena.hpp"

namespace cbb {

namespace parser {

struct NodeId {
    size_t value;

    explicit NodeId(size_t value);
    NodeId();

    bool operator==(const NodeId &other) const;

    NodeId &operator+=(const size_t &other);
    NodeId &operator++();
    NodeId operator++(int);

    friend std::ostream &operator<<(std::ostream &os, const NodeId &nodeid);
};

struct NodeIdHasher {
    size_t operator()(const NodeId &id) const noexcept;
};

struct Expr;

enum class ExprType {
    BinaryOp,
    UnaryOp,
    Literal,

    Error,
};

enum class BinaryOpType {
    Add = 0x00,
    Sub = 0x01,
    Mul = 0x02,
    Div = 0x03,
};

enum class UnaryOpType {
    Negate,
};

using BinOp = core::Spanned<BinaryOpType>;
using UnaryOp = core::Spanned<UnaryOpType>;

struct BinaryOperator {
    Expr *left;
    Expr *right;
    BinOp op;
};

struct UnaryOperator {
    Expr *operand;
    UnaryOp op;
};

enum class LiteralType {
    Integer,
};

struct Literal {
    LiteralType type;
    union {
        u64 integer;
    } as;

    Literal(u64 i);
};

struct Expr {
    ExprType type;
    NodeId id;
    core::Span span;
    union {
        BinaryOperator binop;
        UnaryOperator unaryop;
        Literal literal;
    } as;

    Expr(NodeId id, core::Span span, BinaryOperator binop);
    Expr(NodeId id, core::Span span, UnaryOperator unaryop);
    Expr(NodeId id, core::Span span, Literal literal);
    // for error node
    Expr(NodeId id, core::Span span);
};

struct Program {
    core::arena arena;
    Expr *base;
};

std::ostream &operator<<(std::ostream &os, const BinaryOperator &op);
std::ostream &operator<<(std::ostream &os, const UnaryOperator &op);
std::ostream &operator<<(std::ostream &os, const Literal &literal);
std::ostream &operator<<(std::ostream &os, const Expr &expr);
std::ostream &operator<<(std::ostream &os, const Program &program);
std::ostream &operator<<(std::ostream &os, const UnaryOpType &type);
std::ostream &operator<<(std::ostream &os, const BinaryOpType &type);
std::ostream &operator<<(std::ostream &os, const ExprType &type);

} // namespace parser

} // namespace cbb

#endif // CBB_PARSE_AST_HPP