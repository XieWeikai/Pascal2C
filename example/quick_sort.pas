program QuickSort;
var
  arr : array[1..100] of integer;
  len, i : integer;

procedure swap(var a, b: integer);
var
  temp: integer;
begin
  temp := a;
  a := b;
  b := temp;
end;

procedure QuickSort(l, r: integer);
var
  i, j, p: integer;
begin
  if l < r then
  begin
    i := l;
    j := r;
    p := arr[(l + r) div 2];

    while i <= j do
    begin
      while arr[i] < p do
        i := i + 1;
      while arr[j] > p do
        j := j - 1;

      if i <= j then
      begin
        swap(arr[i], arr[j]);
        i := i + 1;
        j := j - 1;
      end;
    end;

    if l < j then
      QuickSort(l, j);
    if i < r then
      QuickSort(i, r);
  end;
end;

begin
  read(len);
  for i := 1 to len do
    read(arr[i]);

  QuickSort(1, len);

  for i := 1 to len do
    write(arr[i], ' ');
  writeln;
end.
