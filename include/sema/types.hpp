#ifndef CBB_SEMA_TYPES_HPP
#define CBB_SEMA_TYPES_HPP

#include <array>
#include <utility>

#include "parse/ast.hpp"

namespace cbb {

namespace sema {

struct AnalysisCtx;

enum class Type {
    Int,

    Error,
};

class TypeChecker {
public:
    TypeChecker() = default;

    Type checkExpr(parser::Expr *expr, AnalysisCtx *ctx);

private:

    Type resolveBinaryArithmetic(parser::BinaryOpType op, Type left, Type right);

    Type resolveLogicalUnary(parser::UnaryOpType op, Type operand);

    using BinaryOpTypeHandler = Type(TypeChecker::*)(parser::BinaryOpType, Type, Type);
    using UnaryOpTypeHandler = Type(TypeChecker::*)(parser::UnaryOpType, Type);


    BinaryOpTypeHandler getBinaryHandler(parser::BinaryOpType op);
    UnaryOpTypeHandler getUnaryHandler(parser::UnaryOpType op);

    static constexpr std::array<std::pair<std::pair<Type, Type>, Type>, 2> additionTypeTable = {{
        {{Type::Int, Type::Int}, Type::Int},

        {{Type::Error, Type::Error}, Type::Error},
    }};

    static constexpr std::array<std::pair<std::pair<Type, Type>, Type>, 2> subtractionTypeTable = {{
        {{Type::Int, Type::Int}, Type::Int},

        {{Type::Error, Type::Error}, Type::Error},
    }};

    static constexpr std::array<std::pair<std::pair<Type, Type>, Type>, 2> multiplicationTypeTable = {{
        {{Type::Int, Type::Int}, Type::Int}
    }};

    static constexpr std::array<std::pair<std::pair<Type, Type>, Type>, 2> divisionTypeTable = {{
        {{Type::Int, Type::Int}, Type::Int}
    }};

    static constexpr std::array<std::pair<Type, Type>, 1> unaryNegateTable = {{
        {Type::Int, Type::Int}
    }};
};

} // namespace sema

} // namespace cbb 

#endif // CBB_SEMA_TYPES_HPP