program merge_sort;

const
    n = 10;

var
    mas: array [1..n] of integer;
    i: integer;

function random(a : integer):integer;
begin
    random := 3;
end;

procedure inc(var a : integer);
begin
	a := a+1;
end;

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