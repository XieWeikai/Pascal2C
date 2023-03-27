## 1 任务说明

本项目需要设计实现一个编译器，该编译器的源语言为pascal-S语言，目标语言为C语言，要求完成以下功能

- 将合法的pascal源代码转化为等价的c语言代码。
- 若输入存在语法错误，能指出语法错误的位置并提供描述。
- 若出现类型不匹配、函数参数不匹配等错误，编译器能指出错误位置并描述。

任务示例源程序如下

```pascal
program ADD;
var 
	a,b, res : real;
procedure ReadOprand;
  begin
    read(a,b)
  end;
  
function add(a,b : real) : real ;
	begin
		add = a + b
	end;

begin
	ReadOprand;
	write(add(a,b))
end.
```

翻译后的目标代码示例如下

```c
#include <stdio.h>

double a,b;

void ReadOprand(){
  scanf("%lf%lf",&a,&b);
}

double add(double a,double b){
  double res;
  res = a + b;
  return res;
}

int main(){
  ReadOprand();
  add(a,b)
  return 0;
}
```

---