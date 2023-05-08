program Adder;

var
	a , b , c : integer;

function add(x , y : integer) :integer;
begin
	add := x + y;
end;

begin
	a := 1 + 2;
	b := 3 + 4;
	c := add(a , b);
end.