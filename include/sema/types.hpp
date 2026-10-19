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

    Type resolveArithmetic(parser::BinaryOpType op, Type left, Type right);

    using BinaryOpTypeHandler = Type(TypeChecker::*)(parser::BinaryOpType, Type left, Type right);

    BinaryOpTypeHandler getBinaryHandler(parser::BinaryOpType op);

    static constexpr std::array<BinaryOpTypeHandler, 1> binaryoptypehandlers = { 
        &TypeChecker::resolveArithmetic,
    };

    static constexpr std::array<std::pair<std::pair<Type, Type>, Type>, 1> additionTypeTable = {{
        {{Type::Int, Type::Int}, Type::Int}
    }};

    static constexpr std::array<std::pair<std::pair<Type, Type>, Type>, 1> subtractionTypeTable = {{
        {{Type::Int, Type::Int}, Type::Int}
    }};

    static constexpr std::array<std::pair<std::pair<Type, Type>, Type>, 1> multiplicationTypeTable = {{
        {{Type::Int, Type::Int}, Type::Int}
    }};

    static constexpr std::array<std::pair<std::pair<Type, Type>, Type>, 1> divisionTypeTable = {{
        {{Type::Int, Type::Int}, Type::Int}
    }};
};

} // namespace sema

} // namespace cbb 

#endif // CBB_SEMA_TYPES_HPP