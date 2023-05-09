## 5 测试

### 5.1 词法分析

词法分析模块使用Google Test进行单元测试，测试用例位于`test/lexer/lexer_test.cc`中. 以下是各个测试套件的简介.

另外, 为了方便交互式测试, 词法分析模块还提供了一个交互式测试程序 `lexer_exe`, 可以从标准输入读入Pascal源代码, 并将词法分析结果输出到标准输出.

#### 5.1.1 简单测试套件 (LexerSimpleTest)

该测试套件用于测试词法分析器在最基本的输入情况下能否正常工作.

##### 简单测试 (SimpleTest)

输入:

```
1 2 3 4
5 6 7 8
```

预期输出:

8个TOK_INTEGER以及对应的行列号和值.

##### Hello World测试 (HelloWorld)

输入:

```
program hello;
begin
    write('Hello, world!\n');
end.
```

预期输出:

每个TOKEN的类型及其对应的行列号. 对于标识符和字符串, 还需要输出其值.

#### 5.1.2 注释测试套件 (LexerCommentTest)

该套件用于测试词法分析器在遇到注释时能否正确跳过注释.

##### 简单注释测试 (SimpleComment)

输入:

```
begin{comment here}begin
```

预期输出:

一个TOK_BEGIN, 一个TOK_BEGIN, 成功跳过注释.

##### 嵌套注释测试 (NestedComment)

输入:

```
begin{comment here{comment here}}begin
```

预期输出:

一个TOK_BEGIN, 一个TOK_BEGIN, 成功跳过注释.

##### 复杂注释测试 (ComplexComment)

输入:

```
{{{{}}}}
begin {begin{end
}}             begin  {begin
}
begin
```

预期输出:

三个TOK_BEGIN, 成功跳过注释.

#### 5.1.3 标识符测试套件 (LexerIdentifierTest)

该套件用于测试词法分析器在遇到各种标识符时能否正确识别.

##### 合法标识符测试 (ValidIdentifier)

输入:

```
begin
    var a, a1, _a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6q_: integer;
    writeln('Valid identifiers');
end.
```

预期输出:

能够正确识别所有合法标识符.

##### 大小写不敏感测试 (CaseInsensitiveTest)

输入:

```
var Identifier, identifier, IDENTIFIER, IdEnTiFiEr: integer;
```

预期输出:

输出四个TOK_ID, 值均为identifier. (Pascal语言中标识符大小写不敏感)

#### 5.1.4 整数测试套件 (LexerIntegerTest)

##### 简单整数测试 (SimpleInteger)

输入:

```
12345, 9494949, 0;
```

预期输出:

识别出其中的三个TOK_INTEGER, 值分别为12345, 9494949, 0.

##### 整数长度测试 (IntegerLengthTest)

输入:

```
2147483647, 2147483648
```

预期输出:

第一个TOK_INTEGER的值为2147483647, 第二个输出为TOK_ERROR, 表示整数溢出.

#### 5.1.5 实数测试套件 (LexerRealTest)

##### 简单实数测试 (SimpleRealTest)

输入:

```
123.456, 123., 123.0, 123.0e10, 123.0e+10, 123.0e-10;
```

预期输出:

识别出其中的六个TOK_REAL, 值分别为123.456, 123., 123.0, 123.0e10, 123.0e+10, 123.0e-10.

##### 实数省略测试 (RealOmitNumberTest)

输入:

```
123.e10, .1e+10;
```

预期输出:

识别出其中的两个TOK_REAL, 值分别为123.0e10, 0.1e+10.

---

### 5.2 语法分析

#### 5.2.1 测试环境

语法分析模块的测试环境是在 Linux 下使用 Google Test 进行单元测试，测试用例位于`test/parser`目录下。
#### 5.2.1 测试功能

语法分析模块的测试目标是保证语法分析器能够正确的将输入的 token 序列解析成 AST 并且能够正确的处理各种语法错误。具体包括以下几个部分：

* 声明部分解析测试（ProgramParserTest）
  * 程序分析测试（TestParseProgram）
  * 程序头部分析测试（TestParseProgramHead）
  * 程序体分析测试（TestParseProgramBody）
  * 常量声明分析测试（TestParseConstDecl）
  * 变量声明分析测试（TestParseVarDecl）
  * 子程序声明分析测试（TestParseSubProgramDecl）
  * 子程序头部分析测试（TestParseSubProgramHead）
  * 子程序体分析测试（TestParseSubProgramBody）
  * 标识符列表分析测试（TestParseIdList）
  * 类型分析测试（TestParseType）
  * 数组上下界分析测试（TestParsePeriod）
  * 参数分析测试（TestParseParameter）
* 表达式分析测试（ExprParserTest）
  * 表达式因子解析测试（TestParsePrimary）
  * 复杂表达式解析测试（TestParseExpr）
  * 表达式错误检测测试 (TestParserErr)
* 语句分析测试（StatementParserTest）
  * 赋值语句解析测试（TestAssignStatement）
  * if语句分析测试（TestIfStatement）
  * for语句分析测试（TestForStatement）
  * 复合语句分析测试（TestCompoundStatement）
  * 语句解析错误处理测试（TestStatementErr）
* 总体分析测试（TotalParserTest）

各个部分的解析采用手写递归下降的方式来实现，每种语言结构的解析都有对应的方法/函数来完成解析成AST的操作。为了保证代码的正确性，需要对上面提到的各个部分均进行充分的测试。

#### 5.2.2 单元测试用例

对于parser的各个小功能需要分别进行测试以保证整体功能的正确性，在做测试时需要尽可能的保证覆盖率。

##### 程序分析测试（TestParseProgram）

```c++
const vector<string> input_strs = {
    "program f(a, b); var a, b : integer; begin  end.",
    "program test; var a:integer; begin end.",
    "program test var a: integer; begin end.",
    "program test const a = 1; begin end.",
    "program test procedure p;begin end; begin end.",
    "program test function f : integer ;begin end; begin end.",
    "program test begin end.",
    "program test; begin end",
};
const vector<string>
    results = {
        "1:1 Program: \n"
        "1:1     ProgramHead: f\n"
        "1:11         IdList: a, b\n"
        "1:18     ProgramBody: \n"
        "1:22         VarDeclaration: \n"
        "1:29             Type: integer\n"
        "1:22             IdList: a, b\n"
        "        1:38 CompoundStatement :0",
        "1:1 Program: \n"
        "1:1     ProgramHead: test\n"
        "\n"
        "1:15     ProgramBody: \n"
        "1:19         VarDeclaration: \n"
        "1:21             Type: integer\n"
        "1:19             IdList: a\n"
        "        1:30 CompoundStatement :0",
        "1:1 Program: \n"
        "1:1     ProgramHead: test\n"
        "\n"
        "1:14     ProgramBody: \n"
        "1:18         VarDeclaration: \n"
        "1:21             Type: integer\n"
        "1:18             IdList: a\n"
        "        1:30 CompoundStatement :0",
        "1:1 Program: \n"
        "1:1     ProgramHead: test\n"
        "\n"
        "1:14     ProgramBody: \n"
        "1:20         ConstDeclaration: a\n"
        "            1:24 1\n"
        "        1:27 CompoundStatement :0",
        "1:1 Program: \n"
        "1:1     ProgramHead: test\n"
        "\n"
        "1:14     ProgramBody: \n"
        "1:14         Subprogram: \n"
        "1:14             SubprogramHead: procedure p\n"
        "\n"
        "1:26             SubprogramBody: \n"
        "                1:26 CompoundStatement :0\n"
        "        1:37 CompoundStatement :0",
        "1:1 Program: \n"
        "1:1     ProgramHead: test\n"
        "\n"
        "1:14     ProgramBody: \n"
        "1:14         Subprogram: \n"
        "1:14             SubprogramHead: function f integer\n"
        "\n"
        "1:36             SubprogramBody: \n"
        "                1:36 CompoundStatement :0\n"
        "        1:47 CompoundStatement :0",
        "1:1 Program: \n"
        "1:1     ProgramHead: test\n"
        "\n"
        "1:14     ProgramBody: \n"
        "        1:14 CompoundStatement :0",
        "1:1 Program: \n"
        "1:1     ProgramHead: test\n"
        "\n"
        "1:15     ProgramBody: \n"
        "        1:15 CompoundStatement :0",
    };
const vector<vector<string>> errs = {
    {},
    {},
    {
        "1:14 syntax err:expected ';' before 'var'",
    },
    {
        "1:14 syntax err:expected ';' before 'const'",
    },
    {
        "1:14 syntax err:expected ';' before 'procedure'",
    },
    {
        "1:14 syntax err:expected ';' before 'function'",
    },
    {
        "1:14 syntax err:expected ';' before 'begin'",
    },
    {
        "1:21 syntax err:expected '.' before end of file",
    },
};
```

|测试用例|预期结果|测试错误|
|---|---|---|
|program f(a, b); var a, b : integer; begin  end.|Program 包含 ProgramHead 和 ProgramBody 两个节点；ProgramHead 的名称为 f，包含一个 IdList 节点；IdList 含有 a 和 b 两个 id；ProgramBody 含有一个 VarDeclaration 节点和一个 Statement 节点；VarDeclaration 含有一个 IdList 节点和一个 Type 节点；IdList 含有 a 和 b 两个 id；Type 为 integer 基础类型|无|
|program test; var a:integer; begin end.| Program 包含 ProgramHead 和 ProgramBody 两个节点；ProgramHead 的名称为 test，不包含 IdList 节点；ProgramBody 含有一个 VarDeclaration 节点和一个 Statement 节点；VarDeclaration 含有一个 IdList 节点和一个 Type 节点；IdList 含有 a 一个 id；Type 为 integer 基础类型|无|
| program test var a: integer; begin end.| Program 包含 ProgramHead 和 ProgramBody 两个节点；ProgramHead 的名称为 test，不包含 IdList 节点；ProgramBody 含有一个 VarDeclaration 节点和一个 Statement 节点；VarDeclaration 含有一个 IdList 节点和一个 Type 节点；IdList 含有 a 一个 id；Type 为 integer 基础类型|syntax err:expected ';' before 'var'|
|program test; const a = 1; begin end.|Program 包含 ProgramHead 和 ProgramBody 两个节点；ProgramHead 的名称为 test，不包含 IdList 节点；ProgramBody 含有一个 ConstDeclaration 节点和一个 Statement 节点；ConstDeclaration 的 id 为 a，包含一个 Expression 节点；Expression 为 1|syntax err:expected ';' before 'const'|
|program test; procedure p; begin end.|Program 包含 ProgramHead 和 ProgramBody 两个节点；ProgramHead 的名称为 test，不包含 IdList 节点；ProgramBody 含有一个 Subprogram 节点和一个 Statement 节点；Subprogram 含有一个 SubprogramHead 节点和一个 SubprogramBody 节点；SubprogramHead 为 procedure，名称为 p，不包含 Parameter 节点；SubprogramBody 含有一个 Statement 节点；|syntax err:expected ';' before 'procedure'|
|program test; function f; begin end.|Program 包含 ProgramHead 和 ProgramBody 两个节点；ProgramHead 的名称为 test，不包含 IdList 节点；ProgramBody 含有一个 Subprogram 节点和一个 Statement 节点；Subprogram 含有一个 SubprogramHead 节点和一个 SubprogramBody 节点；SubprogramHead 的名称为 f，不包含 IdList 节点；SubprogramBody 含有一个 CompoundStatement 节点；CompoundStatement 为空|syntax err:expected ';' before 'function'|
|program test; begin end.|Program 包含 ProgramHead 和 ProgramBody 两个节点；ProgramHead 的名称为 test，不包含 IdList 节点；ProgramBody 含有一个 CompoundStatement 节点；CompoundStatement 为空|syntax err:expected ';' before 'begin'|
|program test; begin end.|Program 包含 ProgramHead 和 ProgramBody 两个节点；ProgramHead 的名称为 test，不包含 IdList 节点；ProgramBody 含有一个 CompoundStatement 节点；CompoundStatement 为空|syntax err:expected '.' before end of file|


##### const declarations

该部分为program header后接的部分或子程序原型后接的部分，声明全局作用域或局部作用域下的常量。设计测试用例如下

```pascal
const a = 'b';

const a = 1; b = -2; ch = 'b';

const a; b = -2; ch = 'a';     { 语法错误 }

const ;                        { 语法错误 }
```

##### variable declarations

在程序开头或子程序开头常量声明(如果有的话)之后可以接变量声明，对变量声明解析设计测试如下

```pascal
var a, b: integer;
		x, y: real; ch: char;
		mx: array [3..9] of integer;

var a:integer;
		b:integer;
		arr: array [1..2 , 3..4 , 5..9] of real;
		
var a,b integer;      {语法错误}

var c : array [1...3] of integer;   { 语法错误 }

var c : array [1..10,] of integer ;  { 语法错误 }

var c : array [1..10] integer;       { 语法错误 }

var a;                               { 语法错误 }
```

##### 表达式解析测试(ExprParserTest)

表达式解析是递归下降解析中最有难度的部分，需要考虑运算符的结合性、优先级、括号等。在`Parser`中较为重要的两个方法为`ParsePrimary`和`ParseExpr`，`ParsePrimary`方法可以解析出一个表达式最基本的构成部分，如`3` `1.234` `-3` `add(3,4)` `true` `false` `abc` `a[10]`等等；`ParseExpr`方法可以解析出一整个表达式.在`ExprParserTest`中，我们对这两个方法进行了测试，同时也测试了碰到错误的表达式时`Parser`的表现。

**TestParsePrimary**
测试样例如下
```pascal
3  1.23  -3  not 4 +4  -4.1234 abcd add(3,4) count[i+1,b+2] say() true false not true
```
|测试用例| 预期结果 |
|:---:|:---:|
|3|解析出一个整数，整数值为3|
|1.23|解析出一个实数，实数值为1.23|
|-3|解析出一个`UnaryExpr`节点，op为'-' `factor`为3对应ast节点|
|not 4|解析出一个`UnaryExpr`节点，op为'not' `factor`为4对应ast节点|
|4    |解析出一个整数，整数值为4|
|+4   |解析出一个`UnaryExpr`节点，op为'+' `factor`为4对应ast节点|
|-4.1234|解析出一个`UnaryExpr`节点，op为'-' `factor`为4.1234对应ast节点|
|abcd|解析出一个`CallOrVar`节点，id为abcd|
|add(3,4)|解析出一个`CallValue`节点，id为add，params包含两个`Expr`节点，分别对应3和4|
|count[i+1,b+2]|解析出一个`Variable`节点，id为count，expr_list包含两个`Expr`节点，分别对应i+1和b+2|
|say()|解析出一个`CallValue`节点，id为say，params为空|
|true|解析出一个`Boolean`节点，value为true|
|false|解析出一个`Boolean`节点，value为false|
|not true|解析出一个`UnaryExpr`节点，op为'not' `factor`为true对应ast节点|

测试结果符合预期，测试通过。

**TestParseExpr**
测试样例如下
```pascal
1 + 2 + 3 + 4;
1 + 2 * 3  ;
(1 + 2) * 3 ;
-(1 + 2) * 3  ;
(-(1 + 2) * 3 <= 5) and (3 > 4) or (4 < 3) ;
(-(1 + 2) * 3 <= 5) or (3 > 4) and (4 < 3) ;
1 + 'a' + 'abc' + a + b ;
true or false and true ;
(true or false) and true ;
true or (false and true) ;
```

上述各个表达式解析的结果应该满足`pascal`中的运算符优先级结合规则，并且括号能改变运算结合顺序。通过`ToString`方法测试结果是否符合预期，最终通过测试。

**TestParserErr**
const char *input_strs[] = {
                "a + ",  // 不完整表达式
                "(a+1",  // 括号不匹配
                "a + * 1 ",  // 表达式错误
                "a[]",     // 数组下标错误
                "a[1",     // 数组下标错误
        };
|测试用例| 预期结果 |
|:---:|:---:|
|a + |在+后parser希望碰到值或id，但没有，故解析出错，抛出异常|
|(a+1|括号不匹配，抛出异常|
|a + * 1 |表达式错误，+后面希望碰到值或id，但碰到了*，故解析错误，抛出异常|
|a[]|数组下标错误希望碰到数值或id，但碰到了]，故解析错误，抛出异常|
|a[1|中括号未正确闭合，抛出异常|

最终测试结果符合预期，测试通过。

##### 语句解析测试(StatementParserTest)

目前支持解析的语句有如下几种:
- compound statement: 复合语句
- assignment statement: 赋值语句
- if statement: if语句
- for statement: for语句
- call statement: 子程序调用语句

对这些语句的解析设计测试程序如下:

**TestAssignStatement**

样例输入:    
```pascal
a := a + 100 ;
abc := 1 + 2 + 3 ;
count[1,2] := 1 + 2 * 3;
res := add(i+1,b+3) * factor() ;
```
|测试用例| 预期结果 |
|:---:|:---:|
|a := a + 100 ;|解析出一条赋值语句，其中var部分对应a，expr部分对应a + 100这个表达式|
|abc := 1 + 2 + 3 ;|解析出一条赋值语句，其中var部分对应abc，expr部分对应1 + 2 + 3这个表达式|
|count[1,2] := 1 + 2 * 3;|解析出一条赋值语句，其中var部分对应count[1,2]，expr部分对应1 + 2 * 3这个表达式|
|res := add(i+1,b+3) * factor() ;|解析出一条赋值语句，其中var部分对应res，expr部分对应add(i+1,b+3) * factor()这个表达式|

上述各个测试样例解析结果都是正确的，因此测试通过。

**TestIfStatement**

样例输入:    
```pascal
if a <= limit then
   a := a + 100;
if i <= 10 then
   i := i + 1
else\n"
   i := i - 1;
```
|测试用例| 预期结果 |
|:---:|:---:|
|if a <= limit then a := a + 100;|解析出一条if语句，其中condition部分对应a <= limit，then部分对应a := a + 100这条语句|
|if i <= 10 then i := i + 1 else i := i - 1;|解析出一条if语句，其中condition部分对应i <= 10，then部分对应i := i + 1，else部分对应i := i - 1|

上述各个测试样例解析结果都是正确的，因此测试通过。

**TestForStatement**

样例输入:    
```pascal
for i := 1 to 10 do
   count := count + 1;
for i := lower(i) to upper(i) do
begin
   sub_program;
   call(1,2,3);
   a := a + 1
end
```
|测试用例| 预期结果 |
|:---:|:---:|
|for i := 1 to 10 do count := count + 1;|解析出一条for语句，其中id部分对应i，from部分对应1，to部分对应10，do部分对应count := count + 1这条语句|
|for i := lower(i) to upper(i) do begin sub_program; call(1,2,3); a := a + 1 end|解析出一条for语句，其中var部分对应i，from部分对应lower(i)，to部分对应upper(i)，do部分对应begin sub_program; call(1,2,3); a := a + 1 end这个复合语句|

上述各个测试样例解析结果都是正确的，因此测试通过。

**TestCompoundStatement**
样例输入
```pascal
begin
   if l < r then
       begin
           mid := (l + r) / 2;
           sort(l,mid);
           sort(mid+1,r)
       end
   else
       write(a,b,c + d)
end;
```
这个测试用例中有两个`CompoundStatement`，一个就是外层的`begin` `end`包围的语句，该`CompoundStatement`由一条`if`语句构成，而`if`语句的`then`部分是一个`CompoundStatement`,该`CompoundStatement`由三条语句构成，分别是`mid := (l + r) / 2;` `sort(l,mid);` `sort(mid+1,r)`.因此用该测试样例不仅可以测试能否正常解析`CompoundStatement`，还可以测试`CompoundStatement`嵌套的情况的解析。

最终解析结果与预期一致，测试通过。

**TestErrorHandle**
在对整个`pascal`源码进行解析时，`subprogram body`和`program body`其实就是一个`CompoundStatement`，因此在解析`subprogram body`和`program body`时，会调用`ParseCompoundStatement`，而在解析语句碰到语法错误时，我们需要能够记录下该错误并能够继续解析，因此我们需要在`ParseCompoundStatement`中进行错误处理，为了测试错误处理效果设计测试样例如下
```pascal
begin
    a : = 1 + 2;
    for := 1 to 10 do
        subprogram;
    if 3 <> 4 then
        a := 3 + 4;

    const a = 'b';
    var c = 'd' 

    my_func (1,2+3;
    c := 1 + 2 + *

    d := (3 * 4 -( 5 - 3))

    if (3 - (4) > c then
        write('hello')

    for c := a to b do begin
        write(a,b,c);
    end
    c[3,4,7 := 4
end.
```
`ParseCompoundStatement`解析上面的语句后应当产生如下错误信息
- "2:7 syntax error: lost ':=' when parsing assign statement",
- "3:9 syntax error: missing id in for statement",
- "8:5 syntax error: declaration is not part of statement",
- "8:13 syntax error: lost ':=' when parsing assign statement",
- "9:5 syntax error: declaration is not part of statement",
- "9:11 syntax error: lost ':=' when parsing assign statement",
- "11:19 syntax error: unclosed parentheses",
- "12:18 syntax error: parse expression error: no expected token",
- "16:5 syntax error: missing ';' at the end of statement",
- "16:21 syntax err:expected ')' before 'then'",
- "19:5 syntax error: missing ';' at the end of statement",
- "21:5 last statement should not end with ;",
- "22:5 syntax error: missing ';' at the end of statement",
- "22:13 syntax error: unclosed brackets"

经过测试后发现`Parser`可以正确提示上面的错误信息，因此测试通过。


##### procedure

procedure包括过程原型声明、常量声明、变量声明、过程体这几部分，其中常量声明、变量声明、过程体解析的测试在前面部分已经设计过，在这部分专注于过程原型声明的解析上，设计测试样例如下

```pascal
procedure go(a,b : integer);

procedure hello;

procedure abc(var a,b : integer; var c,d : real);

procedure cde(a : integer; var d : real);

procedure efg(var d : real; a,b :integer);
```

##### function

function和procedure类似，同样我们设计的测试样例专注于原型声明上，设计测试用例如下

```pascal
function go(a,b : integer) : integer;

function hello : real;

function abc(var a,b : integer; var c,d : real) : real;

function cde(a : integer; var d : real) boolean ;

function efg(var d : real; a,b :integer) real;
```

---

#### 5.2.3 综合测试

做完各个部分的单元测试后，需要将各个部分整合为一个完整的parser，然后做集成测试，生成完整的AST，设计一个综合测试用例如下

```pascal
program merge_sort;

const
    n = 10;

var
    mas: array [1..n] of integer;
    i: integer;

procedure MergeSort(a, c: integer);
var
    x, j, i, n1, n2: integer;
    rez: array[1..1000] of integer;
begin
    if c <= a then 
        exit 
    else 
    begin
        x := (a + c) div 2;
        MergeSort(a, x);
        MergeSort(x + 1, c);
        n1 := a;
        n2 := x + 1;
        for i := a to c do 
        begin
            if (n1 < x + 1) and ((n2 > c) or (mas[n1] < mas[n2])) then
            begin
                rez[i] := mas[n1];
                inc(n1);
            end 
            else 
            begin
                rez[i] := mas[n2];
                inc(n2);
            end;
        end;
        for j := a to c do
            mas[j] := rez[j];
    end; 
end;

begin
    for i := 1 to n do
        mas[i] := random(20);
    writeln(mas);
    MergeSort(1, n);
    writeln(mas);
end.
```

---

### 5.3 语义分析

在保证输入AST有效的前提下，可将输入代码转换为AST观察输出进行测试。

输入1：

```pascal
program Test1;
var
  x: integer;
  y: real;
begin
  x := 10;
  y := 3.14;
  y := x;
end
```

结果1：

通过，无输出。

输入2：

```pascal
program Test2;
var
  x: integer;
  y: real;
begin
  x := 10;
  y := 3.14;
  x := y;
end
```

结果2：

通过，输出以下提示：

```pascal
[WARNING]:隐式类型转换可能导致溢出
```

输入3：

```pascal
program Test3;
var
  x: integer;
  y: real;
begin
  x := 10;
  y := 3.14;
  z := y;
end
```

结果3：

不通过，输出以下提示:

```pascal
[ERROR]:未定义的变量名
```

---

### 5.4 代码生成

为了方便测试，在前三部分测试完毕后，直接以pascal源代码作为输入，生成AST来测试代码生成模块，测试样例如下

```pascal
program merge_sort;

const
    n = 10;

var
    mas: array [1..n] of integer;
    i: integer;

procedure MergeSort(a, c: integer);
var
    x, j, i, n1, n2: integer;
    rez: array[1..1000] of integer;
begin
    if c <= a then 
        exit 
    else 
    begin
        x := (a + c) div 2;
        MergeSort(a, x);
        MergeSort(x + 1, c);
        n1 := a;
        n2 := x + 1;
        for i := a to c do 
        begin
            if (n1 < x + 1) and ((n2 > c) or (mas[n1] < mas[n2])) then
            begin
                rez[i] := mas[n1];
                inc(n1);
            end 
            else 
            begin
                rez[i] := mas[n2];
                inc(n2);
            end;
        end;
        for j := a to c do
            mas[j] := rez[j];
    end; 
end;

begin
    for i := 1 to n do
        mas[i] := random(20);
    writeln(mas);
    MergeSort(1, n);
    writeln(mas);
end.
```

### 5.5 测试方法

本项目考虑采用google test或类似的测试框架进行代码的测试。

### 5.6 集成测试

在分别完成各个模块的测试后，将各个模块组合起来，直接测试能否将Pascal转化为C语言，若该部分测试通过，则本项目基本完成。