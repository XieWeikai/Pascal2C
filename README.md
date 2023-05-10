# Pascal2C

## 程序使用方法

本项目实现了一个pascal语言子集`pascal-s`向`c`语言转化的编译器，最终产生可执行文件为`pascal2c`，该可执行文件使用方式如下

```bash
pascal2c <input_file> [output_file]
```

其中第一个命令行参数为输入的`pascal-s`源文件，第二个参数可选，指定程序输出文件，若不指定输出文件，默认输出文件为`a.c`。若源文件存在错误，`pascal2c`会输出详细错误信息并终止代码生成过程，若源文件代码正常，则将生成代码写入输出文件中，`pascal2c`不会有任何提示输出。

---

下面通过两个示例说明使用方式。

### 示例1

编写一个求最大公因数的`pascal`程序如下

```pascal
{gcd.pas}
program gcd;

var x,y : integer;

function gcd(a,b : integer) : integer;
begin
    if b = 0 then gcd := a
    else gcd := gcd(b, a mod b)
end;

begin
    read(x,y);
    write('gcd(',x,',',y,') = ');
    writeln(gcd(x,y))
end.
```

在`pascal2c`所在目录下执行如下命令

```bash
./pascal2c path/to/gcd.pas
```

由于没有指定输出文件，默认输出在`a.c`中，查看`a.c`文件内容如下

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int x;
int y;
int gcd(int a, int b) {
    int ret_gcd;/* Auto Generated */
    if ((b == 0)) {
        ret_gcd = a;
    } else {
        ret_gcd = gcd(b, (a % b));
    }
    return ret_gcd;/* Auto Generated */
}
// gcd
int main(int argc, char* argv[]) {
    scanf("%d%d", &x, &y);
    printf("%s%d%c%d%s", "gcd(", x, 44, y, ") = ");
    printf("%d\n", gcd(x, y));
    return 0;
}
```

### 示例2

编写求阶乘的`pascal-s`源文件如下

```pascal
{factorial.pas}
program fac;

var n: integer;

function factorial(n: integer): integer;
begin
  if n = 0 then
    factorial := 1
  else
    factorial := n * factorial(n - 1)
end;

function factorial2(n: integer): integer;
var
  i: integer;
begin
    factorial2 := 1;
    for i := 1 to n do
        factorial2 := factorial2 * i
end;

function factorial3(n: integer): integer;
begin
  if n = 0 then
  begin
    factorial3 := 1;
    exit
  end;
  factorial3 := n * factorial3(n - 1)
end;

begin
    read(n);
    writeln(factorial(n));
    writeln(factorial2(n));
    writeln(factorial3(n))
end.
```

在`pascal2c`所在目录下执行如下命令

```bash
./pascal2c path/to/factorial.pas fac.c
```

产生`c`源文件`fac.c`，内容如下

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int n;
int factorial(int n) {
    int ret_factorial;/* Auto Generated */
    if ((n == 0)) {
        ret_factorial = 1;
    } else {
        ret_factorial = (n * factorial((n - 1)));
    }
    return ret_factorial;/* Auto Generated */
}
int factorial2(int n) {
    int ret_factorial2;/* Auto Generated */
    int i;
    ret_factorial2 = 1;
    for (i = 1; i <= n; i++) {
        ret_factorial2 = (ret_factorial2 * i);
    }
    return ret_factorial2;/* Auto Generated */
}
int factorial3(int n) {
    int ret_factorial3;/* Auto Generated */
    if ((n == 0)) {
        ret_factorial3 = 1;
        return ret_factorial3;
    }
    ret_factorial3 = (n * factorial3((n - 1)));
    return ret_factorial3;/* Auto Generated */
}
// fac
int main(int argc, char* argv[]) {
    scanf("%d", &n);
    printf("%d\n", factorial(n));
    printf("%d\n", factorial2(n));
    printf("%d\n", factorial3(n));
    return 0;
}
```

---

## 编译方法

本项目编译环境如下:

- 语言标准：C++ 17
- `cmake`最低要求: cmake 3.10
- 词法分析器生成器`flex`
- `python3`

编译最好在`Linux`下进行(windows下有些编译器不支持`google test`中的一些特性)。

在项目根目录下执行如下命令

```bash
mkdir build  # 创建构建目录
cd build     # 进入构建目录
cmake ..     # 创建构建文件
make         # 编译生成可执行文件
```

编译后`build`目录下有产生如下几个可执行文件

- `pascal2c`：将`pascal-s`翻译为`c`的编译器。
- `lexer_exe`：可交互的词法分析测试程序。
- `lexer_test`：词法分析测试程序。
- `ast_test`： `ast`节点方法测试程序。
- `parser_test`：语法分析测试程序。
- `generator_test`、`transformer_test`：代码生成测试程序。

其中`lexer_exe`从以标准输入流为输入，输出各个`token`，可用于交互式测试，其余测试程序均使用`google_test`生成，直接执行可以看到测试结果，测试内容见测试报告文档。

---

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

---

## 成员分工

- 词法分析：尹睿
- 语法分析：谢卫凯   陈宇
- 语义分析：韩雷  王佳禾
- 代码生成：林梓超  吴清柳
