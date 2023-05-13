## 代码清单

本项目结构如下所示

```bash
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── doc
│   ├── demand_analysis.md
│   ├── design.md
│   ├── overall_design.md
│   ├── summary.md
│   ├── task.md
│   └── test.md
├── example
│   ├── example3.pas
│   └── integration
│       ├── complex_expression.pas
│       ├── error.pas
│       ├── factorial.pas
│       ├── features.pas
│       ├── gcd.pas
│       ├── general.pas
│       ├── mergesort.pas
│       └── quicksort.pas
├── src
│   ├── ast
│   │   ├── ast.h
│   │   ├── expr.cc
│   │   ├── expr.h
│   │   ├── program.cc
│   │   ├── program.h
│   │   ├── statement.cc
│   │   └── statement.h
│   ├── code_generation
│   │   ├── abstract_symbol_table_adapter.h
│   │   ├── ast_adapter.cc
│   │   ├── ast_adapter.h
│   │   ├── code_generator.cc
│   │   ├── code_generator.h
│   │   ├── optimizer
│   │   │   ├── CMakeLists.txt
│   │   │   ├── calculater.cc
│   │   │   ├── calculater.h
│   │   │   ├── opti_common.h
│   │   │   ├── opti_worker.cc
│   │   │   ├── opti_worker.h
│   │   │   ├── transformer.cc
│   │   │   └── transformer.h
│   │   ├── symbol_item.cc
│   │   ├── symbol_item.h
│   │   ├── symbol_table_adapter.cc
│   │   ├── symbol_table_adapter.h
│   │   ├── token_adapter.cc
│   │   ├── token_adapter.h
│   │   ├── type_adaper.cc
│   │   ├── type_adaper.h
│   │   ├── visitor.cc
│   │   └── visitor.h
│   ├── lexer
│   │   ├── lexer.lex
│   │   ├── main.c
│   │   ├── token_generator.py
│   │   └── utils.c
│   ├── main.cc
│   ├── parser
│   │   ├── expr.cc
│   │   ├── parser.cc
│   │   ├── parser.h
│   │   ├── program.cc
│   │   └── statement.cc
│   ├── semantic_analysis
│   │   ├── CMakeLists.txt
│   │   ├── errors.cc
│   │   ├── errors.h
│   │   ├── semantic_analysis.cc
│   │   ├── semantic_analysis.h
│   │   ├── symbol_table.cc
│   │   └── symbol_table.h
│   └── utils.hpp
└── test
    ├── ast
    │   └── get_type_test.cc
    ├── code_generation
    │   ├── array_test.cc
    │   ├── ast_adapter_test.cc
    │   ├── code_generation_test.cc
    │   ├── const_declaration_test.cc
    │   ├── for_statement_test.cc
    │   ├── function_test.cc
    │   ├── if_statement_test.cc
    │   ├── opti
    │   │   └── test.cc
    │   ├── subprogram_test.cc
    │   ├── symbol_table_mock.h
    │   ├── symbol_table_test.cc
    │   └── transformer_test.cc
    ├── lexer
    │   └── lexer_test.cc
    └── parser
        ├── expr_parser_test.cc
        ├── program_parser_test.cc
        ├── statement_parser_test.cc
        └── total_parser_test.cc
```

其内容说明如下：

- `CMakeList.txt`：`cmake`构建文件。
- `doc`：该目录下为文档。
- `example`：该目录下为完整的用于测试的`pascal-s`源码样例。
- `src`：该目录下存有程序所有逻辑功能实现代码
  - `main.cc utils.hpp`：主程序代码，辅助头文件。
  - `ast`：该目录下存放着`AST`各个节点的定义和相关方法的实现代码。
  - `lexer`：该目录下存放着词法分析相关实现代码。
  - `parser`：该目录下存放着语法分析相关实现代码。
  - `semantic_analysis`：符号表和语义分析相关实现代码。
  - `code_generation`：代码生成相关实现代码。
- `test`：该目录下存放所有测试代码
  - `ast`：`AST`节点方法测试代码。
  - `lexer`：词法分析测试代码。
  - `parser`：语法分析测试代码。
  - `code_generation`：代码生成测试代码。