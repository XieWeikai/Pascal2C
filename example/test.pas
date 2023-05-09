program test_c;

const PI = 3.1415;
var b : integer;
    c : real;
    d : array[4..8] of integer;

procedure inc(var a : integer);
begin
    a := a + 1
end;

function incr(a : integer) : integer;
const increment = 1;
begin
    incr := a + increment
end;

function test() : real;
begin
    test := 9.0
end;

begin
    inc(b);
    c := test;
    write(PI);
    d[5] := 5
end.
