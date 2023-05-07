#include <iostream>
#include <stdio.h>
#include "parser/parser.h"
#include "semantic_analysis/semantic_analysis.h"
#include "utils.hpp"

using namespace pascal2c;

std::string filename;

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
        PrintError(lines, "Parser", err.line(), err.col(), err.err_msg());
    }

    // >>>>>> semantic analysis <<<<<<
    analysiser::init();
    analysiser::DoProgram(*program);

    auto analysis_errs = analysiser::GetErrors();
    for (auto &err : analysis_errs) {
        PrintError(lines, "Semantic", err.line(), err.column(), err.msg());
    }

    return 0;
}
