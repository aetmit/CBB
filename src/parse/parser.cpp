#include "parse/parser.hpp"

#include <cerrno>
#include <cstdlib>

#include "core/error.hpp"

namespace cbb {

namespace parser {

Parser::Parser(const char *source, std::vector<core::Diagnostic> *diagnostics): 
    tokenstream(source), source(source), diagnostics(diagnostics) {
    
    current = tokenstream.next();
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
    return parseFactor();
}

Expr *Parser::parseFactor() {
    return parseUnary();
}

Expr *Parser::parseUnary() {
    return parseUnary();
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
    }
}

} // namespace parser

} // namespace cbb
