#include "parse/parser.hpp"

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include "core/error.hpp"

namespace cbb {

namespace parser {

Parser::Parser(const char *source, std::vector<core::Diagnostic> *diagnostics): 
    tokenstream(source), source(source), diagnostics(diagnostics) {
    
    current = tokenstream.next();
    currentNodeId = NodeId(0);
}

Parser::~Parser() {
    arena = nullptr;
    source = nullptr;
    diagnostics = nullptr;
}

using core::Arena;
using lexer::Token;
using lexer::TokenType;
using core::Span;
using core::Severity;

Program Parser::parse() {
    Program program{};
    this->arena = &program.arena;

    Expr *expr = parseExpression();

    if (peek().type != TokenType::Eof) {
        error("expected eof", false, peek().span);
    }

    program.base = expr;

    return program;
}

void Parser::error(const char *message, bool isAllocated, Span span) {
    diagnostics->emplace_back(Severity::Error, message, isAllocated, span);
}

Token Parser::peek() const {
    return current;
}

Token Parser::advance() {
    Token tmp = peek();
    current = tokenstream.next();
    return tmp;
}

NodeId Parser::nextNodeId() {
    NodeId tmp = currentNodeId;
    currentNodeId.value++;
    return tmp;
}

Expr *Parser::parseExpression() {
    return parseTerm();
}

Expr *Parser::parseTerm() {
    Expr *left = parseFactor();
    Span startSpan = left->span;
    while (peek().type == TokenType::Plus || peek().type == TokenType::Minus) {
        Token tokOp = advance();
        
        Expr *right = parseFactor();

        BinaryOpType opKind;
        if (tokOp.type == TokenType::Plus) {
            opKind = BinaryOpType::Add;
        } else {
            opKind = BinaryOpType::Sub;
        }

        BinOp op(opKind, tokOp.span);

        BinaryOperator binop{ .left = left, .right = right, .op = op };

        Expr *expr = arena->make<Expr>(nextNodeId(), startSpan.join(right->span), binop);
        left = expr;
    }

    return left;
}

Expr *Parser::parseFactor() {
    Expr *left = parseUnary();
    Span startSpan = left->span;
    while (peek().type == TokenType::Star || peek().type == TokenType::Slash) {
        Token tokOp = advance();

        Expr *right = parseUnary();

        BinaryOpType opKind;
        if (tokOp.type == TokenType::Star) {
            opKind = BinaryOpType::Mul;
        } else {
            opKind = BinaryOpType::Div;
        }

        BinOp op(opKind, tokOp.span);

        BinaryOperator binop{ .left = left, .right = right, .op = op };

        Expr *expr = arena->make<Expr>(nextNodeId(), startSpan.join(right->span), binop);
        left = expr;
    }
    return left;
}

Expr *Parser::parseUnary() {
    if (peek().type == TokenType::Minus) {
        Span opSpan = advance().span;

        Expr *operand = parseUnary();

        UnaryOp op(UnaryOpType::Negate, opSpan);
        UnaryOperator unaryop{ .operand = operand, .op = op };

        Expr *expr = arena->make<Expr>(nextNodeId(), opSpan.join(operand->span), unaryop);
        return expr;
    }

    return parsePrimary();
}

Expr *Parser::parsePrimary() {
    if (peek().type == TokenType::IntegerLiteral) {
        Token tok = advance();

        char *lexeme = tok.span.retrieve_source(source);

        errno = 0;

        unsigned long long result = strtoull(lexeme, NULL, 10);
        
        if (errno == ERANGE) {
            core::fatal_error("integer overflow");
        }

        u64 value = static_cast<u64>(result);

        delete[] lexeme;

        Literal literal(value);
        Expr *expr = arena->make<Expr>(nextNodeId(), tok.span, literal);
        return expr;
    } else if (peek().type == TokenType::OpenParen) {
        Span startSpan = advance().span;

        Expr *expr = parseExpression();

        if (peek().type != TokenType::CloseParen) {
            Span span = advance().span;
            error("expected ')' after expression", false, span);

            Expr *expr = arena->make<Expr>(nextNodeId(), span);
            return expr;
        }

        Span closeSpan = advance().span;
        expr->span = expr->span.join(startSpan.join(closeSpan));

        return expr;
    } else {
        Span span = peek().span;
        char *lexeme = span.retrieve_source(source);
        const char *msg = "unexpeced token: ";
        size_t msgLen = std::strlen(msg);
        size_t lexemeLen = std::strlen(lexeme);
        size_t len = msgLen + lexemeLen + 1;
        char *buff = new char[len];
        std::memcpy(buff, msg, msgLen);
        std::memcpy(buff, lexeme, lexemeLen);
        buff[len] = '\0';
        error(buff, true, span);

        Expr *expr = arena->make<Expr>(nextNodeId(), span);
        return expr;
    }
}

} // namespace parser

} // namespace cbb
