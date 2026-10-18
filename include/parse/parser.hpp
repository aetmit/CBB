#ifndef CBB_PARSE_PARSER_HPP
#define CBB_PARSE_PARSER_HPP

#include "parse/ast.hpp"

namespace cbb {

namespace parse {

class Parser {
public:
    Parser(const char *source);
    Program parse();

};

} // namespace parser

} // namespace cbb

#endif // CBB_PARSE_PARSER_HPP