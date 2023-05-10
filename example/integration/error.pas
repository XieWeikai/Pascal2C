program error;

const b;

var a : integer;

procedure test(var a : integer);
begin
	a := a + 1
end;

begin
	a := 3 + 4
	a := 3.5 + 3.2 * 3;
	test(a);
	test(3);
end.