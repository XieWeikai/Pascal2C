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
#include <iostream>
#include <fstream>
using std::string;
using std::make_shared;
using namespace pascal2c::code_generation;
using namespace pascal2c::parser;

TEST(GeneratorTest, TransformerTest) {

    FILE *input = fopen("../example/example1.pas", "r");
    std::cerr <<"[Parser] Begin\n";

	Parser par{input};
	auto ast = par.Parse();
    for (const auto& it : par.err_msg()) {
        std::cerr << it << "\n";
    }
    printf("[Parser] Done\n");

    analysiser::init();
    analysiser::DoProgram(*ast);

    auto analysis_errs = analysiser::GetErrors();
    for (auto &err : analysis_errs) {
        std::cerr <<"@" <<  err.line() <<": "<< err.column() << " " << err.msg() << "\n";
    }

    printf("[Transformer] Start\n");
	Transformer trans{ast};
	auto program = trans.GetASTRoot();
    printf("[Transformer] Done\n");

    auto code_generator = make_shared<CodeGenerator>();
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

    std::ofstream output;
    output.open("../example/out1.c" , std::ios::out);
    output << generated_ccode;
    std::cout << generated_ccode << "\n";
    output.close();
    EXPECT_EQ(generated_ccode, expected_c_code);
}