#include "lexer/token.hpp"

namespace cbb {

namespace lexer {

token::token(token_type type, core::span span, std::size_t column, std::size_t line): type(type), span(span), column(column), line(line) {}

std::ostream &operator<<(std::ostream &os, const token &token) {
    os << "type: ";
    os << token.type;
    os << "\n";
    os << "offset: ";
    os << token.span.offset();
    os << "\nlength: ";
    os << token.span.length();
    os << "\ncolumn: ";
    os << token.column;
    os << "\nline: ";
    os << token.line;
    os << "\n";

    return os;
}

std::ostream& operator<<(std::ostream& os, const token_type& type) {
    switch (type) {
        case token_type::integer_literal:
            os << "integer_literal";
            break;

        case token_type::identifier:
            os << "identifier";
            break;

        case token_type::plus:
            os << "plus";
            break;

        case token_type::minus:
            os << "minus";
            break;

        case token_type::star:
            os << "star";
            break;

        case token_type::slash:
            os << "slash";
            break;

        case token_type::open_paren:
            os << "open_paren";
            break;

        case token_type::close_paren:
            os << "close_paren";
            break;

        case token_type::open_brace:
            os << "open_brace";
            break;

        case token_type::close_brace:
            os << "close_brace";
            break;

        case token_type::equal:
            os << "equal";
            break;

        case token_type::int_:
            os << "int";
            break;

        case token_type::semicolon:
            os << "semicolon";
            break;

        case token_type::eof:
            os << "eof";
            break;

        case token_type::bool_:
            os << "bool";
            break;

        case token_type::true_:
            os << "true";
            break;

        case token_type::false_:
            os << "false";
            break;

        case token_type::bang:
            os << "bang";
            break;

        case token_type::logical_and:
            os << "logical and";
            break;
        
        case token_type::logical_or:
            os << "logical or";
            break;

        case token_type::less:
            os << "less";
            break;

        case token_type::less_eq:
            os << "less equal";
            break;

        case token_type::greater:
            os << "greater";
            break;
        
        case token_type::greater_eq:
            os << "greter equal";
            break;

        case token_type::bang_equal:
            os << "bang equal";
            break;

        case token_type::equal_equal:
            os << "equal equal";
            break;

        default:
            os << "unknown";
            break;
    }

    return os;
}

bool token::operator==(const token &other) const {
    return type == other.type && span == other.span && column == other.column && line == other.line;
}

bool token::operator!=(const token &other) const {
    return !(*this == other);
}
    
} // namespace lexer

    
} // namespace cbb