program general;

const PI = 3.14;

{面积}
procedure area (r: real;var s:real);
const MY_PI = 3.14;
var r2:real;
begin
    r2 := r*r;
    s := MY_PI * r2
end;

{周长}
function circle (r:real):real;
begin
    circle := 2 * PI * r
end;

function expression (x:real):real;
const scalar = 2;
var y:real;
    flag:boolean;
begin
    y := scalar * PI * x;
    y :=  (3 div 2) / 6;
    expression := (1 + 2 * 3) * (4 + 5) - 7 * 8 - 9 * 10;
    flag := (true and false) or (true and true) or (4 < y) and (y < 5) or (y = 6) or (y <> 7) or (y >= 8) or (y <= 9);
    writeln(flag)
end;

function greater (x,y:real):boolean;
begin
    greater := x > y
end;

begin
    writeln
end.