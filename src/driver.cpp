#include "driver.hpp"

#include "core/fs.hpp"
#include "lex/lexer.hpp"

namespace cbb {

using core::FileReader;
using core::File;

int run(int argc, const char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    FileReader reader(argv[1]);
    File file = reader.readFile();

    lexer::Token token;
    lexer::Lexer lexer(file.contents());
    while ((token = lexer.next()).type != lexer::TokenType::Eof) {
        std::cout << token << "\n";
    }

    return 0;
}

} // namespace cbb