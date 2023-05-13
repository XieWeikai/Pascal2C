# Pascal2C用户使用手册

## 介绍

Pascal2C 是一个实现从 Pascal 语言子集 `pascal-s` 向 `c` 语言转换的编译器。它的主要目的和功能是将 Pascal 语言的源代码转换为等效的 C 语言代码，以便在 C 语言环境中编译和运行。

## 安装

### 使用预编译的二进制文件

在项目的 release 中提供了预编译的二进制文件，适用于 `linux-x64` 架构。

可执行文件包括：

- `pascal2c`：编译器主程序
- `lexer_exe`：可交互的词法分析测试程序
- `lexer_test`：词法分析测试程序
- `ast_test`： `ast` 节点方法测试程序
- `parser_test`：语法分析测试程序
- `generator_test`、`transformer_test`：代码生成测试程序

其中，`lexer_exe` 以标准输入流为输入，输出各个 `token`，可用于交互式测试。其余测试程序均使用 `google_test` 生成，直接执行可以查看测试结果。测试内容详见测试报告文档。

### 自行编译

本项目使用 `cmake` 管理编译过程，请确保已经安装了 `cmake`。

编译要求：

- 语言标准：C++ 17
- `cmake` 最低要求：cmake 3.10
- 词法分析器生成器：`flex`
- `python3`

使用以下命令编译:

```bash
mkdir build
cd build
cmake ..
make
```

## 使用方法

```bash
./pascal2c <input_file> [output_file]
```

其中，`input_file` 是输入文件，`output_file` 是输出文件，默认为 `a.c`。

例如，我们有以下 `pascal-s` 代码：

```pascal
{gcd.pas}
program gcd;

var x, y : integer;

function gcd(a, b : integer) : integer;
begin
    if b = 0 then gcd := a
    else gcd := gcd(b, a mod b)
end;

begin
    read(x, y);
    write('gcd(', x, ',', y, ') = ');
    writeln(gcd(x, y))
end.
```

在 `pascal2c` 所在目录下执行如下命令：

```bash
./pascal2c path/to/gcd.pas
```

由于没有指定输出文件，默认输出在 `a.c` 中，查看 `a.c` 文件内容如下：

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

其他示例代码请参见 `example` 目录下的 `pascal-s` 源文件。

## 许可证

本项目及文档使用 `MIT` 许可证，详见 `LICENSE` 文件。
