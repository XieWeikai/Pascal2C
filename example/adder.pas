program Adder;

var
	a , b , c : integer;

function add() :integer;
begin
    a := 5;
    b := 9;
	add := 5 + 6;
end;

begin
	a := 1 + 2;
	b := 3 + 4;
    c := add;
    writeln(b);
end.