#include "sema/types.hpp"

#include <limits>
#include <cstdio>

#include "core/error.hpp"

namespace cbb {

namespace sema {

namespace ast = parser;

Type TypeChecker::checkExpr(ast::Expr *expr, AnalysisCtx *ctx) {
    Type type;
    switch (expr->type) {
        case ast::ExprType::Literal: {
            switch (expr->as.literal.type) {
                case ast::LiteralType::Integer:
                    if (expr->as.literal.as.integer > std::numeric_limits<u64>::max()) {
                        type = Type::Int;
                    }
                    break;
            }
            break;
        }
        case ast::ExprType::BinaryOp: {
            Type leftType = checkExpr(expr->as.binop.left, ctx);
            Type rightType = checkExpr(expr->as.binop.right, ctx);

            BinaryOpTypeHandler handler = getBinaryHandler(expr->as.binop.op.node);
            if (handler == nullptr) {
                throw;
            }
            Type result = (this->*handler)(expr->as.binop.op.node, leftType, rightType);

            type = result;
            break;
        }
        case ast::ExprType::UnaryOp: {
            Type operandType = checkExpr(expr->as.unaryop.operand, ctx);

            
        }
        case ast::ExprType::Error: {
            UNREACHABLE();
        }
    }

    return type;
}

Type TypeChecker::resolveArithmetic(ast::BinaryOpType op, Type left, Type right) {
    Type type;
    switch (op) {
        case ast::BinaryOpType::Add: {
            for (const auto &[k, v] : additionTypeTable)  {
                if (k == std::pair{left, right}) {
                    type = v;
                }
            }
        }
        case ast::BinaryOpType::Sub: {
            for (const auto &[k, v] : subtractionTypeTable)  {
                if (k == std::pair{left, right}) {
                    type = v;
                }
            }
        }
        case ast::BinaryOpType::Mul: {
            for (const auto &[k, v] : multiplicationTypeTable)  {
                if (k == std::pair{left, right}) {
                    type = v;
                }
            }
        }
        case ast::BinaryOpType::Div: {
            for (const auto &[k, v] : divisionTypeTable)  {
                if (k == std::pair{left, right}) {
                    type = v;
                }
            }
        }
    }
    return type;
}

TypeChecker::BinaryOpTypeHandler TypeChecker::getBinaryHandler(ast::BinaryOpType op) {
    switch (op) {
        case ast::BinaryOpType::Add:
        case ast::BinaryOpType::Sub:
        case ast::BinaryOpType::Mul:
        case ast::BinaryOpType::Div:
            return binaryoptypehandlers[0];

        default:
            return nullptr;
    }
}

} // namespace sema

} // namespace cbb