#include "lexer/lexer.hpp"

#include <string> 
#include <algorithm>

#include <cctype>

#include "core/error.hpp"

namespace cbb {

namespace lexer {

lexer::lexer(std::string_view source): source_(source), current_(0), start_(0), line_(0), column_(0), start_column_(0) {}

token lexer::next() {
    if (is_at_end()) {
        return token(token_type::eof, core::span(current_, 0), column_, line_);
    }

    skip_whitespace();

    if (is_at_end()) {
        return token(token_type::eof, core::span(current_, 0), column_, line_);
    }

    start_ = current_;
    start_column_ = column_;
    char c = peek();

    switch (c) {
        case '(':
            advance();
            return create_token(token_type::open_paren);
        case ')':
            advance();
            return create_token(token_type::close_paren);
        case '+':
            advance();
            return create_token(token_type::plus);
        case '-':
            advance();
            return create_token(token_type::minus);
        case '*':
            advance();
            return create_token(token_type::star);
        case '/':
            advance();
            return create_token(token_type::slash);
        case '{':
            advance();
            return create_token(token_type::open_brace);
        case '}':
            advance();
            return create_token(token_type::close_brace);
        case '=':
            advance();
            return create_token(token_type::equal);
        case ';':
            advance();
            return create_token(token_type::semicolon);
        default:
            if (std::isdigit(static_cast<unsigned char>(c))) {
                while (!is_at_end() && std::isdigit(static_cast<unsigned char>(peek()))) {
                    advance();
                }
                return create_token(token_type::int_);
            } else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                std::string lexeme;
                while (!is_at_end() && (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')) {
                    lexeme.push_back(peek());
                    advance();
                }
                token_type identifier_type = get_identifier_type(lexeme);
                return create_token(identifier_type);
            } else {
                FATAL("handle this: {}", static_cast<int>(c));
            }
    }
}

token_type lexer::get_identifier_type(std::string_view lexeme) const {
    for (auto it = keywords_.begin(); it != keywords_.end(); ++it) {
        if (it->lexeme == lexeme) {
            return it->type;
        }
    }

    return token_type::identifier;
}

token lexer::create_token(token_type type) const {
    core::span span(start_, current_ - start_);
    return { type, span, start_column_, line_ };
}

void lexer::skip_whitespace() {
    while (!is_at_end() && std::isspace(static_cast<unsigned char>(peek()))) { 
        advance();
    }
}

char lexer::peek() const {
    if (is_at_end()) {
        return eof_char_;
    }
    return source_[current_];
}

char lexer::advance() {
    char c = peek();
    if (is_at_end()) {
        return c;
    }

    if (c == '\n') {
        column_ = 0;
        line_++;
    } else {
        column_++;
    }
    current_++;
    return c;
}

bool lexer::is_at_end() const {
    return current_ >= source_.size(); 
}

lexer::iterator::iterator(lexer *lexer, token current): lexer_(lexer), current_(current) {}

lexer::iterator::reference lexer::iterator::operator*() const {
    return current_;
}

lexer::iterator &lexer::iterator::operator++() {
    current_ = lexer_->next();
    return *this;
}

lexer::iterator lexer::iterator::operator++(int) {
    iterator &tmp = *this;
    ++(*this);
    return tmp;
}

bool lexer::iterator::operator==(const iterator &other) const {
    return lexer_ == other.lexer_ && current_ == other.current_;
}

bool lexer::iterator::operator!=(const iterator &other) const {
    return !(*this == other);
}

lexer::iterator lexer::begin() {
    return iterator(this, next());
}

lexer::sentinal lexer::end() {  
    return {};
}

lexer::iterator::pointer lexer::iterator::operator->() const {
    return &current_;
}

bool operator==(const lexer::iterator &it, lexer::sentinal) {
    return it.current_.type == token_type::eof;
}
    
} // namespace lexer

    
} // namespace cbb