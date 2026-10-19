#ifndef CBB_SEMA_HIR_HPP
#define CBB_SEMA_HIR_HPP

#include "core/structures/arena.hpp"
#include "core/types.hpp"
#include "sema/ids.hpp"
#include "sema/types.hpp"

namespace cbb {

namespace sema {

struct Expr;

enum class ExprType {
    Literal,
    BinaryOp,
    UnaryOp,
};

enum class BinaryOpType {
    Add,
    Sub,
    Mul,
    Div,
};

struct BinaryOp {
    Expr *left;
    Expr *right;
    BinaryOpType op;
};

enum class UnaryOpType {
    Negate,
};

struct UnaryOp {
    Expr *operand;
    UnaryOpType op;
};

enum class LiteralType {
    Integer,
};

struct Literal {
    LiteralType type;
    union {
        u32 integer;
    } as;

    Literal(u32 i);
};

struct Expr {
    ExprType exprType;
    HirId id;
    Type type;
    union {
        Literal literal;
        UnaryOp unaryop;
        BinaryOp binaryop;
    } as;
};

struct Program {
    core::Arena arena;
    Expr *root;
};

} // namespace sema

} // namespace cbb

#endif // CBB_SEMA_HIR_HPP
