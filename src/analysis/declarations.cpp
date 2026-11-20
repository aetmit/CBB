// #include "analysis/declarations.hpp"

// #include <sstream>

// namespace cbb {

// namespace sema {

// namespace ast = parser;

// collector::collector(): current_functionid_(0) {}

// functionid collector::next_functionid() {
//     functionid tmp = current_functionid_;
//     current_functionid_.value++;
//     return tmp;
// }

// void collector::collect(const ast::program &program, context &ctx) {
//     for (auto it = program.body.begin(); it != program.body.end(); ++it) {
//         collect_function(**it, ctx);
//     }
// }

// void collector::collect_function(const ast::function &function, context &ctx) {
//     if (!identifiers_.insert(function.name).second) {
//         std::ostringstream oss;
//         oss << "redeclaration of "
//         error();
//     }
// }

// } // namespace sema
    
// } // namespace cbb