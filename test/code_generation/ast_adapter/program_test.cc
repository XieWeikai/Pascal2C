#include "ast/ast.h"
#include "ast/program.h"
#include "code_generation/ast_adapter.h"
#include "parser/parser.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cstdio>
#include <memory>

using namespace std;
using namespace pascal2c::ast;
using namespace pascal2c::parser;
using namespace pascal2c::code_generation;

std::shared_ptr<Ast> GetAst(const string &input_str) {
    FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r");
    Parser par(input);
    auto ast = par.Parse();
}

class ProgramTest : public ::testing::Test {
  protected:
    void SetUp() override { auto ast = GetAst(str_program); }
    const string str_program =
        R"(program Simple;
var
    x, y: integer;
begin
    x := 2 + 3;
    y := x - 1;
end.
)";
};