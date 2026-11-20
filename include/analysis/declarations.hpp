// #ifndef CBB_ANALYSIS_DECLARATIONS_HPP
// #define CBB_ANALYSIS_DECLARATIONS_HPP

// #include <unordered_set>

// #include "analysis/ids.hpp"
// #include "analysis/context.hpp"
// #include "parser/ast.hpp"
// #include "core/concepts.hpp"

// namespace cbb {

// namespace sema {

// class collector {
// public:
//     collector();

//     void collect(const parser::program &program, context &ctx);
// private:
//     functionid current_functionid_;
//     std::unordered_set<std::string_view> identifiers_;

//     template <class T>
//     requires core::writable_to_stream<T>
//     void error(T message, core::span span, context &ctx) {
//         std::stringstream ss;
//         ss << message;
//         ctx.diagnostics.push_back(core::diagnostic::error(ss.str(), span));
//     } 

//     functionid next_functionid();

//     void collect_function(const parser::function &function, context &ctx);
// };

// } // namespace sema

// } // namespace cbb

// #endif // CBB_ANALYSIS_DECLARATIONS_HPP