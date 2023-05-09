program merge_sort;

const
    n = 10;

var
    ans: array [1..10] of integer;
    i: integer;
    
procedure inc(var a : integer);
begin
	a := a+1;
end;

procedure MergeSort(l, r: integer);
var
    mid, i, ptr_1, ptr_2: integer;
    tmp: array[1..1000] of integer;
begin
    if r <= l then 
        exit 
    else 
    begin
        mid := (l + r) div 2;
        MergeSort(l, mid);
        MergeSort(mid + 1, r);
        // Merging
        ptr_1 := l;
        ptr_2 := mid + 1;
        for i := l to r do 
        begin
            if (ptr_1 < mid + 1) and ((ptr_2 > r) or (ans[ptr_1] < ans[ptr_2])) then
            begin
                tmp[i] := ans[ptr_1];
                inc(ptr_1);
            end 
            else 
            begin
                tmp[i] := ans[ptr_2];
                inc(ptr_2);
            end;
        end;
        for i := l to r do
            ans[i] := tmp[i];
    end; 
end;

begin
    for i := 1 to n do
        ans[i] := (n-i);
    MergeSort(1, n);
    
    for i := 1 to n do 
        writeln(ans[i]);
end.