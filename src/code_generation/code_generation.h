#ifndef PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
#define PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
#include <string>

// Need to modify after compile_commands.json generated from cmake
#include "../parser/parser.h"
#include "../semantic/ast.h"

namespace pascal2c {
namespace code_generation {
using AST = ::pascal2c::semantic::AST;
using string = ::std::string;
class CodeGenerator {
  public:
    CodeGenerator(const AST &ast) : ast_(ast) {}
    CodeGenerator(const parser::Parser *parser) : parser_(parser) {}
    CodeGenerator() {}
    void Visit(const AST &ast);
    string GetCCode();

  private:
    const parser::Parser *parser_;
    AST ast_;
};
} // namespace code_generation
} // namespace pascal2c
#endif // !PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
