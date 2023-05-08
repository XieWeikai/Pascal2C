#include "code_generation/ast_adapter.h"
#include "code_generation/ast_printer.h"
#include "code_generation/code_generator.h"
#include "code_generation/token_adapter.h"

#include "parser/parser.h"


#include "code_generation/optimizer/transformer.h"

#include "symbol_table_mock.h"
#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using std::string;
using std::make_shared;
using namespace pascal2c::code_generation;
using namespace pascal2c::parser;

TEST(GeneratorTest, TransformerTest) {

    FILE *input = fopen("../example/example1.pas", "r");
	Parser par{input};
	auto ast = par.Parse();
    for (const auto& it : par.err_msg()) {
        std::cerr << it << "\n";
    }

	Transformer trans{ast};
	auto program = trans.getASTRoot();

    auto s_table = make_shared<SymbolTableMock>();
    auto code_generator = make_shared<CodeGenerator>(s_table);
    code_generator->Interpret(program);
    auto generated_ccode = code_generator->GetCCode();

    string expected_c_code = "#include <stdio.h>\n"
                             "#include <stdlib.h>\n"
                             "\n"
                             "// Simple\n"
                             "int main(int argc, char* argv[]) {\n"
                             "    int x;\n"
                             "    int y;\n"
                             "    x = (2 + 3);\n"
                             "    y = (x - 1);\n"
                             "    return 0;\n"
                             "}\n";
    std::cout << generated_ccode << "\n";
    EXPECT_EQ(generated_ccode, expected_c_code);
}