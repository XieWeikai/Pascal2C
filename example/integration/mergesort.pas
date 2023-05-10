program merge_sort;

var n : integer;

var
    mas: array [1..1000] of integer;
    i: integer;

procedure inc(var a: integer);
begin
    a := a + 1;
end;

procedure My_MergeSort(a, c: integer);
var
    x, j, i, n1, n2: integer;
    rez: array[1..1000] of integer;
begin
    if c <= a then
        exit
    else
    begin
        x := (a + c) div 2;
        My_MergeSort(a, x);
        My_MergeSort(x + 1, c);
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
    read(n);
    for i := 1 to n do
        read(mas[i]);

    My_MergeSort(1, n);
    for i := 1 to n do
        write(mas[i], ' ');
    writeln;
end.