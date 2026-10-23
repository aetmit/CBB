#include "parse/parser.hpp"

#include <vector>

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

using lexer::Token;
using lexer::TokenType;
using core::Span;
using core::Severity;

Program Parser::parse() {
    Program program{};
    this->arena = &program.arena;

    std::vector<Stmt*> stmts;

    while (peek().type != TokenType::Eof) {
        stmts.push_back(parseStatement());
    }

    program.body = stmts;

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

void Parser::sync() {
    while (peek().type != TokenType::Eof) {
        TokenType type = peek().type;
        switch (type) {
            case TokenType::Semicolon:
                advance();
                return;
            default:
                advance();
        }
    }
}

NodeId Parser::nextNodeId() {
    return currentNodeId++;
}

Expr *Parser::parseExpression() {
    return parseTerm();
}

Stmt *Parser::parseStatement() {
    switch (peek().type)  {
        case TokenType::IntegerLiteral:
        case TokenType::OpenParen:
            return parseExprStmt();

        default:
            Span span = peek().span;
            const char *lexeme = span.retrieve_source(source);
            const char *start = "unexpected token: ";
            size_t msgStartLen = std::strlen(start);
            size_t lexemeLen = std::strlen(lexeme);
            size_t len = msgStartLen + lexemeLen;
            char *msg = new char[len + 1];
            std::memcpy(msg, start, msgStartLen);
            std::memcpy(msg, lexeme, lexemeLen);
            msg[len + 1] = '\0';

            delete[] lexeme;
            error(msg, true, span);
            sync();
            Stmt *stmt = arena->make<Stmt>(nextNodeId(), span);
            return stmt;
    }
}

Stmt *Parser::parseExprStmt() {
    Expr *expr = parseExpression();

    if (peek().type != TokenType::Semicolon) {
        Span span = peek().span;
        error("expected ';' after expression", false, span);;
        sync();
        Stmt *stmt = arena->make<Stmt>(nextNodeId(), span);
        return stmt;
    }

    Span span = expr->span.join(advance().span);
    ExprStmt exprstmt = { .expr = expr };
    Stmt *stmt = arena->make<Stmt>(nextNodeId(), span, exprstmt);
    return stmt;
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
