#include "code_generation/ast_adapter.h"
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
    FILE *input = fopen("../example/merge_sort.pas", "r");
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

    string expected_c_code = 
R"(#include <stdio.h>
#include <stdlib.h>

const double pi = 3.141590;
int intarray[6];
double realarray[5];
int i;
int add(int x, int y) {
    int ret_add;/* Auto Generated */
    ret_add = (x + y);
    return ret_add;/* Auto Generated */
}
void swap(/* Is Reference */int *x, /* Is Reference */int *y) {
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
void printarrays() {
    int i;
    printf("%s\n", "Integer array:");
    for (i = 1; i <= 5; i++) {
        printf("%s%d%s%d\n", "intArray[", i, "] = ", intarray[i - 1]);
    }
    printf("%s\n", "Real array:");
    for (i = 3; i <= 5; i++) {
        printf("%s%d%s%f\n", "realArray[", i, "] = ", realarray[i - 3]);
    }
}
// compilertest
int main(int argc, char* argv[]) {
    for (i = 1; i <= 5; i++) {
        intarray[i - 1] = (i * 2);
    }
    realarray[3 - 3] = 0.500000;
    realarray[4 - 3] = 1.500000;
    realarray[5 - 3] = 2.500000;
    realarray[6 - 3] = 3.500000;
    realarray[7 - 3] = 4.500000;
    printf("%s\n", "Before swap:");
    printarrays();
    if ((add(intarray[2 - 1], intarray[4 - 1]) == 13)) {
        printf("%s\n", "Swapping intArray[2] and intArray[4]");
        swap(&intarray[2 - 1], &intarray[4 - 1]);
    }
    printf("%s\n", "After swap:");
    printarrays();
    printf("%f\n", pi);
    return 0;
}
)";

    std::ofstream output;
    output.open("../example/out1.c" , std::ios::out);
    output << generated_ccode;
    std::cout << generated_ccode << "\n";
    output.close();
    EXPECT_EQ(generated_ccode, expected_c_code);
}