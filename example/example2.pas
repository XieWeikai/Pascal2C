program CompilerTest;

const
  PI = 3.14159;

var
  intArray: array[1..5] of integer;
  realArray: array[2..2] of real;
  i: integer;

function Add(x, y: integer): integer;
begin
  Add := x + y;
end;

procedure Swap(var x, y: integer);
var
  temp: integer;
begin
  temp := x;
  x := y;
  y := temp;
end;

procedure PrintArrays;
var
  i: integer;
begin
  writeln('Integer array:');
  for i := 1 to 5 do
    writeln('intArray[', i, '] = ', intArray[i]);

  writeln('Real array:');
  for i := -2 to 2 do
    writeln('realArray[', i, '] = ', realArray[i]);
end;

{
  This is a comment.
  { This is a nested comment.
    { This is another nested comment. }
  }
}
begin
  for i := 1 to 5 do
    intArray[i] := i * 2;

  realArray[-2] := 0.5;
  realArray[-1] := 1.5;
  realArray[0] := 2.5;
  realArray[1] := 3.5;
  realArray[2] := 4.5;

  writeln('Before swap:');
  PrintArrays;

  if Add(intArray[2], intArray[4]) = 13 then
  begin
    writeln('Swapping intArray[2] and intArray[4]');
    Swap(intArray[2], intArray[4]);
  end;

  writeln('After swap:');
  PrintArrays;

  writeln('PI = ', PI);
end.
