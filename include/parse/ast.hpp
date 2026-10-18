#ifndef CBB_PARSE_AST_HPP
#define CBB_PARSE_AST_HPP

#include <memory_resource>

#include "core/types.hpp"
#include "core/span.hpp"
#include "core/structures/arena.hpp"

namespace cbb {

namespace parser {

struct NodeId {
    size_t value;

    explicit NodeId(size_t value);
};

struct Expr;

enum class ExprType {
    BinaryOp,
    UnaryOp,
    Literal,

    Error,
};

enum class BinaryOpKind {
    Add,
    Sub,
    Mul,
    Div,
};

enum class UnaryOpKind {
    Negate,
};

using BinOp = core::Spanned<BinaryOpKind>;
using UnaryOp = core::Spanned<UnaryOpKind>;

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
};

struct Program {
    core::Arena arena;
    Expr *base;
};

} // namespace parser

} // namespace cbb

#endif // CBB_PARSE_AST_HPP