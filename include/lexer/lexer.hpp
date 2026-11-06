#ifndef CBB_LEXER_LEXER_HPP
#define CBB_LEXER_LEXER_HPP

#include <string_view>
#include <array>
#include <algorithm>
#include <iterator>

#include <cstddef>

#include "lexer/token.hpp"

namespace cbb {

namespace lexer {

class lexer {
public:
    lexer(std::string_view source);
    
    struct sentinal {};
    class iterator {
    public: 
        iterator(lexer *lexer, token current); 

        using reference = const token&;
        using pointer = const token*;
        using value_type = token;
        using iterator_category = std::input_iterator_tag;
        using iterator_concept  = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;

        reference operator*() const;

        iterator &operator++();
        iterator operator++(int);

        pointer operator->() const;

        bool operator==(const iterator &other) const;
        bool operator!=(const iterator &other) const;

        friend bool operator==(const iterator &it, sentinal);

    private:
        lexer *lexer_;
        token current_;
    };

    iterator begin();
    sentinal end();

private:
    std::string_view source_;

    std::size_t current_;
    std::size_t start_;
    std::size_t line_;
    std::size_t column_;
    std::size_t start_column_;

    token next();

    bool is_at_end() const;
    void skip_whitespace();
    char peek() const;
    char advance();
    token create_token(token_type type) const;
    token_type get_identifier_type(std::string_view lexeme) const;

    struct keyword {
        std::string_view lexeme;
        token_type type;
    };

    static constexpr auto keywords_ = std::array<keyword, 1>{{
        {"int", token_type::int_},
    }};


    // if we advance or peek when we run out of source then this is what is gonna be returned but this hsouldn't even hapeen in the first place
    static constexpr char eof_char_ = '\0';
};

} // namespace lexer

    
} // namespace cbb

#endif // CBB_LEXER_LEXER_HPP