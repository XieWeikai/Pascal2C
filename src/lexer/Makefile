lexer: main.c utils.c lex.yy.c lexer.h
	gcc -o lexer main.c utils.c lex.yy.c
lex.yy.c: lexer.lex
	flex lexer.lex
lexer.h: token_generator.py
	python token_generator.py

.PHONY: clean
clean:
	rm -f lex.yy.c lexer.h lexer