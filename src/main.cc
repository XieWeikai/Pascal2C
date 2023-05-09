#include <iostream>
#include <stdio.h>
#include <map>
#include "parser/parser.h"
#include "semantic_analysis/semantic_analysis.h"
#include "utils.hpp"
#include "code_generation/optimizer/transformer.h"
#include "code_generation/code_generator.h"


using namespace pascal2c;

std::string filename;
using ErrorMsg = std::variant<pascal2c::parser::SyntaxErr, analysiser::errorMsg>;
using Location = std::pair<int, int>;
std::map<Location, ErrorMsg> errors;

void PrintError(const std::vector<std::string> &lines,
                std::string module_name,
                int line, int col, const std::string &msg) {
    const int context_num = 2;
    int start = std::max(1, line - context_num);
    int end = std::min((int) lines.size(), line + context_num);

    std::cout << Colorize("Error (" + module_name + ") -> ", Color::Red) << filename
              << ":" << line << ":" << col << std::endl;

    int num_width = std::to_string(end).size();

    for (int i = start; i <= end; ++i) {
        std::string line_num = std::string(num_width - std::to_string(i).size(), ' ') + std::to_string(i);
        std::cout << Colorize(line_num + " | ", Color::Blue);
        if (i == line) {
            std::cout << lines[i - 1];
            if (lines[i - 1].back() != '\n') {
                std::cout << std::endl;
            }

            int offset = num_width + 3 + col - 1;
            std::cout << std::string(offset, ' ');
            std::cout << Colorize("^", Color::Red);
            std::cout << ' ' << Colorize(msg, Color::Red);
            std::cout << std::endl;
        } else {
            std::cout << lines[i - 1];
        }
    }
    std::cout << std::endl;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 0;
    }

    filename = argv[1];
    FILE *fp = fopen(filename.c_str(), "r");

    // read all lines
    std::vector<std::string> lines;
    size_t max_len = 1024;
    char buf[max_len];
    char *p = buf;
    int ret;
    while (getline(&p, &max_len, fp) != -1) {
        lines.push_back(buf);
    }
    // reset fp
    fseek(fp, 0, SEEK_SET);

    // >>>>>> lexer & parser <<<<<<
    parser::Parser parser(fp);
    auto program = parser.Parse();

    auto parser_errs = parser.syntax_errs();
    for (auto &err : parser_errs) {
        errors.insert({{err.line(), err.col()}, err});
    }

    // >>>>>> semantic analysis <<<<<<
    analysiser::init();
    analysiser::DoProgram(*program);

    auto analysis_errs = analysiser::GetErrors();
    for (auto &err : analysis_errs) {
        errors.insert({{err.line(), err.column()}, err});
    }

    // print errors
    for (auto &err : errors) {
        auto [line, col] = err.first;
        auto msg = err.second;
        if (auto *p = std::get_if<pascal2c::parser::SyntaxErr>(&msg)) {
            PrintError(lines, "Parser", line, col, p->err_msg());
        } else if (auto *p = std::get_if<analysiser::errorMsg>(&msg)) {
            PrintError(lines, "Semantic", line, col, p->msg());
        }
    }

    code_generation::Transformer trans(program);
    auto cg_program = trans.GetASTRoot();
    auto s_table = std::make_shared<code_generation::SymbolTable>();
    auto code_generator = code_generation::CodeGenerator(s_table);

    code_generator.Interpret(cg_program);

    std::cout << code_generator.GetCCode() << std::endl;

    return 0;
}
