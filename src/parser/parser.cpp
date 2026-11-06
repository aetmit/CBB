#include "parser/parser.hpp"

#include <vector>
#include <memory>

namespace cbb {

namespace parser {

parser::parser(std::string_view source): source_(source), tokenstream_(source), current_(tokenstream_.begin()), current_nodeid_(0), arena(nullptr) {}

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

    return { std::move(arena), stmts };
}
    
} // namespace parser

    
} // namespace cbb
