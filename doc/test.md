## 5 测试

### 5.1 词法分析

#### 测试用例1

输入： 

```
program HelloWorld;

var
  x: integer;
  y: real;

begin
  x := 10;
  y := 3.14;
  writeln('Hello, World!');
  writeln('The value of x is ', x);
  writeln('The value of y is ', y:2:2);
end.
```

输出：

```
TokenType: PROGRAM, Line: 1, Column: 1
TokenType: IDENTIFIER, Attribute: 'HelloWorld', Line: 1, Column: 9
TokenType: SEMICOLON, Line: 1, Column: 19
TokenType: VAR, Line: 3, Column: 1
TokenType: IDENTIFIER, Attribute: 'x', Line: 4, Column: 3
TokenType: COLON, Line: 4, Column: 4
TokenType: INTEGER, Line: 4, Column: 6
TokenType: SEMICOLON, Line: 4, Column: 13
TokenType: IDENTIFIER, Attribute: 'y', Line: 5, Column: 3
TokenType: COLON, Line: 5, Column: 4
TokenType: REAL, Line: 5, Column: 6
TokenType: SEMICOLON, Line: 5, Column: 10
TokenType: BEGIN, Line: 7, Column: 1
TokenType: IDENTIFIER, Attribute: 'x', Line: 8, Column: 3
TokenType: ASSIGN, Line: 8, Column: 6
TokenType: INTEGER_CONST, Attribute: '10', Line: 8, Column: 9
TokenType: SEMICOLON, Line: 8, Column: 11
TokenType: IDENTIFIER, Attribute: 'y', Line: 9, Column: 3
TokenType: ASSIGN, Line: 9, Column: 6
TokenType: REAL_CONST, Attribute: '3.14', Line: 9, Column: 9
TokenType: SEMICOLON, Line: 9, Column: 13
TokenType: WRITELN, Line: 10, Column: 3
TokenType: LEFT_PAREN, Line: 10, Column: 9
TokenType: STRING_CONST, Attribute: 'Hello, World!', Line: 10, Column: 10
TokenType: RIGHT_PAREN, Line: 10, Column: 24
TokenType: SEMICOLON, Line: 10, Column: 25
TokenType: WRITELN, Line: 11, Column: 3
TokenType: LEFT_PAREN, Line: 11, Column: 9
TokenType: STRING_CONST, Attribute: 'The value of x is ', Line: 11, Column: 10
TokenType: COMMA, Line: 11, Column: 29
TokenType: IDENTIFIER, Attribute: 'x', Line: 11, Column: 31
TokenType: RIGHT_PAREN, Line: 11, Column: 32
TokenType: SEMICOLON, Line: 11, Column: 33
TokenType: WRITELN, Line: 12, Column: 3
TokenType: LEFT_PAREN, Line: 12, Column: 9
TokenType: STRING_CONST, Attribute: 'The value of y is ', Line: 12, Column: 10
TokenType: COMMA, Line: 12, Column: 29
TokenType: IDENTIFIER, Attribute: 'y', Line: 12, Column: 31
TokenType: COLON, Line: 12, Column: 32
TokenType: INTEGER_CONST, Attribute: '2', Line: 12, Column: 34
TokenType: COLON, Line: 12, Column: 35
TokenType: INTEGER_CONST, Attribute: '2', Line: 12, Column: 37
TokenType: RIGHT_PAREN, Line: 12, Column: 38
TokenType: SEMICOLON, Line: 12, Column: 39
TokenType: END, Line: 13, Column: 1
TokenType: DOT, Line: 13, Column: 5
```

#### 测试用例2

输入:

```pascal
program ErrorExample;

var
  1x: integer; // Error: identifier can't start with a number
  y: integer;

begin
  1x := 12345678901234567890; // Error: integer is too long
  y := 100;
  { This is a comment with no closing delimiter EOF
  // Error: comment not terminated before the end of the file end.
```

输出:

```
TokenType: PROGRAM, Line: 1, Column: 1
TokenType: IDENTIFIER, Attribute: 'ErrorExample', Line: 1, Column: 9
TokenType: SEMICOLON, Line: 1, Column: 21
TokenType: VAR, Line: 3, Column: 1
TokenType: ERROR, Attribute: '1x', Line: 4, Column: 3
TokenType: COLON, Line: 4, Column: 4
TokenType: INTEGER, Line: 4, Column: 6
TokenType: SEMICOLON, Line: 4, Column: 14
TokenType: IDENTIFIER, Attribute: 'y', Line: 5, Column: 3
TokenType: COLON, Line: 5, Column: 5
TokenType: INTEGER, Line: 5, Column: 7
TokenType: SEMICOLON, Line: 5, Column: 14
TokenType: ERROR, Errno: 1, Line: 7, Column: 3
TokenType: ASSIGN, Line: 7, Column: 6
TokenType: ERROR, Errno: 2, Line: 7, Column: 9
TokenType: SEMICOLON, Line: 7, Column: 31
TokenType: IDENTIFIER, Attribute: 'y', Line: 8, Column: 3
TokenType: ASSIGN, Line: 8, Column: 6
TokenType: INTEGER_CONST, Attribute: '100', Line: 8, Column: 9
TokenType: SEMICOLON, Line: 8, Column: 12
TokenType: LEFT_BRACE, Line: 9, Column: 3
TokenType: ERROR, Errno: 3, Line: 9, Column: 4
```

---

### 5.2 语法分析

#### 5.2.1 测试目标

语法分析模块需要将各种程序结构解析成对应的AST树，包括如下一些部分

- program header 的解析。
- const declaration的解析。
- var declaration的解析。
- expression的解析。
- statement的解析。
- procedure的解析。
- function的解析。

各个部分的解析采用手写递归下降的方式来实现，每种语言结构的解析都有对应的方法/函数来完成解析成AST的操作。为了保证代码的正确性，需要对上面提到的各个部分均进行充分的测试。

#### 5.2.2 单元测试用例

对于parser的各个小功能需要分别进行测试以保证整体功能的正确性，在做测试时需要竟可能的保证覆盖率。

##### program header

program header是pascal程序起始部分，声明了程序名和程序参数，设计各种测试样例如下

```pascal
program MyProgram;  {不带id list}

program MyProgram(input);  {id list中有一个id}

program MyProgram(input,output,logfile);  {id list中有多个id}

MyProgram;        {语法错误，没有program关键字 期望报错:filename:line:column: syntax error:expect program}

MyProgram(a,b,c);  { 同上 }

program MyProgram(a,b,);    {语法错误}

program MyProgram(a ;       {语法错误}
```

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

##### expression

表达式解析是递归下降解析中最有难度的部分，需要考虑运算符的结合性、优先级、括号等。设计表达式解析的测试样例如下

```pascal
123

1.234

true

false

abc

cde[2]

efg(1,2.3)

not true

not false

1 + 2 * 3

(1 + 2) * (4 - 2 + a[3]) - add(1,1) <= 34 and 3 > 5 or -(7 * 4) > -288

1 +- 4   {语法错误}

1+(3*(4-(5+5))    {语法错误，括号匹配错误}
```

##### statement

目前支持好几种语句的解析，设计测试样例如下

```pascal
a := <expr>      {expr采用表达式测试时使用的表达式}

add := oprand1 + oprand2  {add 为函数名}

say_hello                 {调用procedure}

say_hello_to(a)           {调用function}

if a > b then <statement>

if a < b then <statement> else <statement>

for a := <expr> to <expr> do <statement>

read(a,b)

write(c+d,add(e,f))

begin
	<statement>;
	<statement>;
	...
	<statement>
end
```

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