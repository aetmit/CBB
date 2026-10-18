#include "parse/parser.hpp"

namespace cbb {

namespace parser {

Parser::Parser(const char *source): tokenstream(source), source(source) {}

Parser::~Parser() {
    arena = nullptr;
    source = nullptr;
}

Program Parser::parse() {
    
}

} // namespace parser

} // namespace cbb
