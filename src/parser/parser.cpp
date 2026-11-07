#include "parser/parser.hpp"

#include <charconv>
#include <utility>

#include "core/error.hpp"

namespace cbb {

namespace parser {

parser::parser(std::string_view source, std::vector<core::diagnostic> &diagnostics): 
    source_(source), tokenstream_(source), current_(tokenstream_.begin()), current_nodeid_(0), arena_(nullptr), diagnostics_(diagnostics) {}

lexer::token parser::peek() const {
    return *current_;
}

lexer::token parser::advance() {
    auto tmp = current_;
    ++current_;
    return *tmp;
}

program parser::parse() {
    auto arena = std::make_unique<core::arena>();

    std::vector<stmt*> stmts;

    arena_ = &arena;

    current_ = tokenstream_.begin();
    while (current_ != tokenstream_.end()) {
        stmts.push_back(parse_statement());
    }

    return { std::move(arena), std::move(stmts) };
}

stmt *parser::parse_statement() {
    switch (peek().type) {
        case lexer::token_type::integer_literal:
        case lexer::token_type::open_paren:
        case lexer::token_type::identifier:
        case lexer::token_type::minus:
            return parse_stmtexpr();
        case lexer::token_type::int_:
            return parse_vardecl();
        case lexer::token_type::open_brace:
            return parse_block();
        default:
            core::span span = peek().span;
            std::stringstream ss;
            ss << "unexpected token: ";
            std::string_view lexeme = span.retrieve_lexeme(source_);
            ss << "'" << lexeme << "'";
            error(ss.str(), span);
            sync();
            
            stmt *stmt = alloc_stmt(span);
            return stmt;
    }
}

stmt *parser::parse_block() {
    core::span start_span = advance().span;

    std::vector<stmt*> stmts;

    while (peek().type != lexer::token_type::eof) {
        if (peek().type == lexer::token_type::close_brace) {
            break;
        }
        stmts.push_back(parse_statement());
    }

    lexer::token end_token = advance();

    if (end_token.type != lexer::token_type::close_brace) {
        error("unterminated block", end_token.span);
        sync();
        
        stmt *stmt = alloc_stmt(end_token.span);
        return stmt;
    }

    block block(stmts);

    stmt *stmt = alloc_stmt(start_span.join(end_token.span), block);
    return stmt;
}

stmt *parser::parse_vardecl() {
    core::span start_span = peek().span;

    type_syntax type = parse_type();
    
    if (peek().type != lexer::token_type::identifier) {
        core::span span = peek().span;

        error("expected identifier", span);

        sync();
        
        stmt *stmt = alloc_stmt(span);
        return stmt;
    }

    core::span ident_span = advance().span;
    std::string_view identifier = ident_span.retrieve_lexeme(source_);

    if (peek().type == lexer::token_type::semicolon) {
        advance();
        var_decl decl(identifier, type, nullptr);
        stmt *stmt = alloc_stmt(start_span.join(ident_span), decl);

        return stmt;
    }

    if (peek().type != lexer::token_type::equal) {
        core::span span = peek().span;

        error("expected '='", span);

        sync();

        stmt *stmt = alloc_stmt(span);
        return stmt;
    }

    advance();

    expr *init = parse_expression();

    if (peek().type != lexer::token_type::semicolon) {
        core::span span = peek().span;

        error("expected ';'", span);

        sync();

        stmt *stmt = alloc_stmt(span);
        return stmt;
    }

    advance();

    var_decl decl(identifier, type, init);
    stmt *stmt = alloc_stmt(start_span.join(init->span), decl);

    return stmt;
}

type_syntax parser::parse_type() {
    if (is_builtin_type(peek().type)) {
        type_syntax_type type = get_builtin_type();

        advance();

        type_syntax type_syntax = { .type = type };

        return type_syntax;
    } else {
        // unimplemented because the only type that exists right now is int
        UNIMPLEMENTED();
    }

}

type_syntax_type parser::get_builtin_type() const {
    if (!is_builtin_type(peek().type)) {
        FATAL("call to cbb::parser::get_builtin_type() with non builtin type");
    }
    using lexer::token_type;
    switch (peek().type) {
        case token_type::int_:
            return type_syntax_type::int_;
        default:
            UNREACHABLE();
    }
}

bool parser::is_builtin_type(lexer::token_type type) const {
    using lexer::token_type;
    
    switch (type) {
        case token_type::int_:
            return true;
        default:
            return false;
    }
}

stmt *parser::parse_stmtexpr() {
    expr *expr = parse_expression();

    exprstmt exprstmt(expr);

    if (peek().type != lexer::token_type::semicolon) {
        core::span span = peek().span;

        error("expected ';'", span);
        sync();
        
        stmt *stmt = alloc_stmt(span);

        return stmt;
    }
    
    advance();

    stmt *stmt = alloc_stmt(expr->span, exprstmt);

    return stmt;
}

expr *parser::parse_expression() {
    return parse_assignment(); 
}

expr *parser::parse_assignment() {
    expr *left = parse_term();

    if (peek().type == lexer::token_type::equal) {
        advance();

        expr *right = parse_assignment();

        core::span span = left->span.join(right->span);

        var_assign var_assign = { .target = left, .value = right };

        expr *expr = alloc_expr(span, var_assign);

        return expr;
    }

    return left;
}

expr *parser::parse_term() {
    expr *left = parse_factor();
    core::span start_span = left->span;
    while (peek().type == lexer::token_type::plus || peek().type == lexer::token_type::minus) {
        lexer::token op = advance();

        expr *right = parse_factor();

        binaryop_type type;
        if (op.type == lexer::token_type::plus) {
            type = binaryop_type::add;
        } else {
            type = binaryop_type::sub;
        }

        binary binary(core::spanned(type, op.span), left, right);

        left = alloc_expr(start_span.join(right->span), binary);
    }

    return left;
}

expr *parser::parse_factor() {
    expr *left = parse_unary();
    core::span start_span = left->span;
    while (peek().type == lexer::token_type::star || peek().type == lexer::token_type::slash) {
        lexer::token op = advance();

        expr *right = parse_unary();

        binaryop_type type;
        if (op.type == lexer::token_type::star) {
            type = binaryop_type::mul;
        } else {
            type = binaryop_type::div;
        }

        binary binary(core::spanned(type, op.span), left, right);

        left = alloc_expr(start_span.join(right->span), binary);
    }

    return left;
}

expr *parser::parse_unary() {
    if (peek().type == lexer::token_type::minus) {
        core::span op_span = advance().span;

        expr *operand = parse_unary();

        unary unary(core::spanned(unaryop_type::negate, op_span), operand);

        expr *expr = alloc_expr(operand->span.join(op_span), unary);

        return expr;
    }

    return parse_primary();
}

expr *parser::parse_primary() {
    if (peek().type == lexer::token_type::integer_literal) {
        lexer::token token = peek();

        std::string_view lexeme = token.span.retrieve_lexeme(source_);

        // lexeme is guarenteed to be a valid number since the lexer makes sure abt that
        // because of that the only error we need to handle is overflow

        uint64_t value;
        auto [ptr, ec] = std::from_chars(lexeme.data(), lexeme.data() + lexeme.size(), value);

        if (ec == std::errc::result_out_of_range) {
            FATAL("integer literal out of range");
        }

        literal literal(value);

        advance();

        expr *expr = alloc_expr(token.span, literal);

        return expr;
    } else if (peek().type == lexer::token_type::open_paren) {
        core::span start_span = advance().span;

        expr *expr = parse_expression();

        if (peek().type != lexer::token_type::close_paren) {
            core::span span = peek().span;
            error("expected ')'", span);
            sync();
            
            struct expr *expr = alloc_expr(span);

            return expr;
        }

        core::span close_span = advance().span;

        expr->span = expr->span.join(start_span.join(close_span));

        return expr;
    } else if (peek().type == lexer::token_type::identifier) {
        std::string_view name = peek().span.retrieve_lexeme(source_);

        core::span span = advance().span;

        variable var = { .name = name };

        expr *expr = alloc_expr(span, var);

        return expr;
    } else {
        core::span span = peek().span;

        std::stringstream ss;
        ss << "unexpected token: ";
        std::string_view lexeme = span.retrieve_lexeme(source_);
        ss << "'" << lexeme << "'";

        error(ss.rdbuf(), span);

        sync();

        expr *expr = alloc_expr(span);

        return expr;
    }
}

void parser::sync() {
    while (peek().type != lexer::token_type::eof) {
        lexer::token_type type = peek().type;
        using lexer::token_type;
        switch (type) {
            case token_type::semicolon:
                advance();
                return;
            case token_type::open_brace:
                return;
            case token_type::close_brace:
                return;
            default:
                advance();
                break;
        }
    }
}

nodeid parser::next_nodeid() {
    nodeid tmp = current_nodeid_;
    current_nodeid_.value++;
    return tmp;
}

expr *parser::alloc_expr(core::span span) {
    void *memory = (*arena_)->allocate(sizeof(expr), alignof(expr));

    expr *expr = new (memory) struct expr(next_nodeid(), span);

    return expr;
}

stmt *parser::alloc_stmt(core::span span) {
    void *memory = (*arena_)->allocate(sizeof(stmt), alignof(stmt)) ;

    stmt *stmt = new (memory) struct stmt(next_nodeid(), span);

    return stmt;
}
    
} // namespace parser

    
} // namespace cbb
