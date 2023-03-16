#ifndef PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
#define PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
#include <string>

// Need to modify after compile_commands.json generated from cmake
#include "ast_interface.h"
#include "parser/parser.h"

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
class CodeGenerator {
  public:
    CodeGenerator(const parser::Parser *parser) : parser_(parser) {}
    void Visit(const semantic::ASTRoot &ast);
    string GetCCode();

  private:
    const parser::Parser *parser_;
    semantic::ASTRoot ast_;
};
} // namespace code_generation
} // namespace pascal2c
#endif // !PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
