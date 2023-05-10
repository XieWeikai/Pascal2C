program test;

const PI = 3.14;
var tmp : integer;

function value() : integer;
begin
    value := 1;
end;

procedure expression;
const scalar = 2;
var
    a : real;
    b : real;
    c : integer;
    flag : boolean;
begin
    c := 9 div 2 * 3 mod 4;
    a := (PI + value * 2 - (4 - 2)) * 2;
    flag := (a > 0) and (a < 10) or (a = 0) or (a = 10) or (a <= 0) and (a >= 10) or (a <> 8) and true and not false;
    b := scalar * 3 / 4 + 2 * (1.6 - 2.5);
    writeln('a = ', a);
    writeln('b = ', b);
    writeln('c = ', c);
    writeln('flag = ', flag);
end;

function var_test(var a : integer; b : integer) : integer;
begin
    a := b;
end;

function exit_test(a : integer) : boolean;
begin
    if a > 0 then
    begin
        writeln('a > 0');
        exit_test := true;
        exit;
    end
    else writeln('a <= 0');

    exit_test := false;
end;

procedure while_test(n : integer);
var i : integer;
begin
    writeln('while_test');
    i := 0;
    while i <= n do
    begin
        writeln('i = ', i);
        i := i + 1;
    end;
end;

function for_test(n : integer) : integer;
var sum : integer;
    i : integer;
begin
    sum := 0;
    for i := 1 to n do
        sum := sum + i;
    for_test := sum;
end;

begin
    expression;
    writeln('expression done.');
    var_test(tmp, 20 + value);
    writeln('var_tmp done. tmp = ', tmp);
    writeln('exit_test exit_test(tmp) = ', exit_test(tmp));
    writeln('exit_test exit_test(-tmp) = ', exit_test(-tmp));
    writeln('exit_test done.');
    while_test(10);
    writeln('while_test done.');
    writeln('for_test for_test(10) = ', for_test(10));
    writeln('for_test done.');
end.
