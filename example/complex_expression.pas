program ComplexExpressionTest;

var
    i: integer;
    r, s, t: real;

function add(x: real; y: integer): real;
begin
    add := x + y;
end;

function square(x: real): real;
begin
    square := x * x;
end;

function increment: integer;
begin
    increment := 1 + 1;
end;

begin
    i := 5;
    r := 2.5;

    s := i + square(r) * add(r, increment) - (i mod increment) / square(r);

    writeln('Result: ', s);
end.
