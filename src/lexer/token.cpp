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