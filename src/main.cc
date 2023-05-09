#include <iostream>
#include <fstream>
#include <stdio.h>
#include <map>
#include "parser/parser.h"
#include "semantic_analysis/semantic_analysis.h"
#include "utils.hpp"
#include "code_generation/optimizer/transformer.h"
#include "code_generation/code_generator.h"


using namespace pascal2c;

std::string input_filename;
using ErrorMsg = std::variant<pascal2c::parser::SyntaxErr, analysiser::errorMsg>;
using Location = std::pair<int, int>;
std::map<Location, ErrorMsg> errors;

void PrintError(const std::vector<std::string> &lines,
                std::string module_name,
                int line, int col, const std::string &msg) {
    const int context_num = 2;
    int start = std::max(1, line - context_num);
    int end = std::min((int) lines.size(), line + context_num);

    std::cerr << Colorize("Error (" + module_name + ") -> ", Color::Red) << input_filename
              << ":" << line << ":" << col << std::endl;

    int num_width = std::to_string(end).size();

    for (int i = start; i <= end; ++i) {
        std::string line_num = std::string(num_width - std::to_string(i).size(), ' ') + std::to_string(i);
        std::cerr << Colorize(line_num + " | ", Color::Blue);
        if (i == line) {
            std::cerr << lines[i - 1];
            if (lines[i - 1].back() != '\n') {
                std::cerr << std::endl;
            }

            int offset = num_width + 3 + col - 1;
            std::cerr << std::string(offset, ' ');
            std::cerr << Colorize("^", Color::Red);
            std::cerr << ' ' << Colorize(msg, Color::Red);
            std::cerr << std::endl;
        } else {
            std::cerr << lines[i - 1];
        }
    }
    std::cerr << std::endl;
}


int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> [output_file]" << std::endl;
        return 0;
    }

    input_filename = argv[1];
    std::string output_filename = argc == 3 ? argv[2] : "a.c";
    FILE *finp = fopen(input_filename.c_str(), "r");
    std::ofstream fout(output_filename);

    // read all lines
    std::vector<std::string> lines;
    size_t max_len = 1024;
    char buf[max_len];
    char *p = buf;
    int ret;
    while (getline(&p, &max_len, finp) != -1) {
        lines.push_back(buf);
    }
    // reset finp
    fseek(finp, 0, SEEK_SET);

    // >>>>>> lexer & parser <<<<<<
    parser::Parser parser(finp);
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

    auto code_generator = code_generation::CodeGenerator();

    code_generator.Interpret(cg_program);

    fout << code_generator.GetCCode() << std::endl;

    return 0;
}
