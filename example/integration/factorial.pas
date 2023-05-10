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