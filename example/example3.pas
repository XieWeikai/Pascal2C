program SATest;

const
  PI = 3.14159;{常量定义}

var
  intVar: integer;{普通变量定义}
  boolVar: boolean;
  realVar: real;
  intArray: array[1..5] of integer;{数组定义}
  realArray: array[2..10] of real;
  intArray2D:array[0..5,1..5] of integer;{多维数组定义}
  intArray3D:array[0..5,0..5,1..5] of integer;
  realArray3D:array[0..5,0..5,1..5] of real;

function Add(x, y: integer): integer;{函数定义}
const 
    PIINADD = 3;{函数内常量定义}
var 
    intVarInAdd:integer;{函数内变量定义}
    intArrayInAdd: array[1..5] of integer;{函数内数组定义}
    realArrayInAdd: array[2..2] of real;
    intArray2DInAdd:array[0..5,1..5] of integer;{函数内多维数组定义}
    intArray2DInAdd2:array[2..5,2..5] of integer;
    intArray3DInAdd:array[0..5,0..5,1..5] of integer;
    realArray3DInAdd:array[0..5,0..5,1..5] of real;
begin
  {正常通过}
  intVarInAdd := PIINADD;{函数内调用内部变量，函数内调用内部常量}
  realVar := PI;{函数内调用外部变量，函数内调用外部常量}
  realVar := intVar;{int赋值real}
  intArray[3] := PIINADD;{一维数组赋值}
  intArray2DInAdd[1,1] := PIINADD;{多维数组赋值}
  intArray3DInAdd[1] := intArray2DInAdd;
  Add := x + y;{数组返回值赋值，数组参数引用}

  {errors}
  intVar := realVar;{real赋值int}
  intArray2DInAdd := intArray3DInAdd;{多维数组赋值:2d=3d}
  intArray2DInAdd2 := intArray2DInAdd;{多维数组赋值:数组大小不匹配}
  intArray2DInAdd[1,1,1] := PIINADD{多维数组引用:2d数组当3d用}
end;

function AddVar(var x: integer):integer;{传引用函数定义}
begin
    AddVar(x){函数递归调用}
end;
function AddVoid : integer;{空返回值函数定义}
begin
    AddVoid:=1
end;
function AddVar(var x,y:integer):integer;{函数重载}
begin
    AddVar(x)
end;
begin
    {正常通过}
    intArray[1] := intVar * 2 + 1 - intArray2D[1,1];{加减乘语句}
    realVar := 5/2;{除语句}
    intVar := 5 div 2;
    intVar := 5 mod 2;{模语句}
    realVar := 5 mod 2; 


    if not ((intVar=1) and (realVar<3) and (realVar>5) or (AddVoid<>0) and (intArray2D[1,2]>=0) and (intArray2D[2,3]<=0)){>,<,<>,=,>=,<=,and,or,not} 
    then 
        intVar := AddVoid + 1{空参数列表函数调用}   
    else 
        realArray[-2] := 0.5;{数组赋值常量}

    for intVar := 1 to 5 do intVar := -1;{‘-’}

    read(intVar);{read函数}
    readln(intVar);{readln函数}
    write('hello world');{write函数}
    writeln('hello world');{writeln函数}
    AddVar(intVar,intVar);{函数调用及重载调用}
    AddVar(intVar);


    {errors}
    
    intVar := 4/2;{'/'做除号返回为非int类}
    PI := 3.14;{常量赋值}
    intVar := not_defined_int;{使用未定义变量}
    intVar:=false;{赋值类型错误}
    intVar:=boolVar;
    boolVar:=AddVoid;

    if intVar then intVar := -1 else intVar := -1;{if语句条件不为bool}

    for boolVar:=false to true do intVar := -1;{for语句非int}

    AddVar(intVar,intVar,intVar);{未定义函数调用}
    not_defined_func(intVar);
    AddVar(AddVoid,1){传引用错误} 
end.
