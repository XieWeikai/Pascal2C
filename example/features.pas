program TestPascal;

const
    PI = 3.14159;

var
    i, j: integer;
    r: real;
    a: array [1..5] of integer;

procedure printArray(hi: integer);
var
    i: integer;
begin
    for i := 1 to hi do
        write(a[i], ' ');
    writeln;
end;

function add(x, y: integer): integer;
begin
    add := x + y;
end;

procedure swap(var x, y: integer);
var
    temp: integer;
begin
    temp := x;
    x := y;
    y := temp;
end;

begin
    writeln('Hello, \'', PI, '\'!');

    i := 5;
    j := 10;
    r := i / j;
    writeln('Real division: ', r);

    r := i div j;
    writeln('Integer division: ', r);

    writeln('Complex expression: ', ((i + j) * i - j) mod i);

    for i := 1 to 5 do
        a[i] := i * i;

    printArray(3);

    writeln('Function add: ', add(i, j));

    writeln('Before swap: ', i, ' ', j);
    swap(i, j);
    writeln('After swap: ', i, ' ', j);

    while i > 0 do begin
        writeln('Countdown: ', i);
        i := i - 1;
    end;
end.
