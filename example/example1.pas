program CompilerTest;

const
  PI = 3.14159;
{
  type
    TIntArray = array[1..5] of integer;
    TRealArray = array[-2..2] of real;
}
var
  intArray: array[1..6] of integer;
  realArray: array[3..7] of real;
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
  for i := 3 to 5 do
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

  realArray[3] := 0.5;
  realArray[4] := 1.5;
  realArray[5] := 2.5;
  realArray[6] := 3.5;
  realArray[7] := 4.5;

  writeln('Before swap:');
  PrintArrays;

  if Add(intArray[2], intArray[4]) = 13 then
  begin
    writeln('Swapping intArray[2] and intArray[4]');
    Swap(intArray[2], intArray[4]);
  end;

  writeln('After swap:');
  PrintArrays;

  writeln(PI);
end.