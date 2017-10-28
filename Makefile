repl: main.c token.c lexer.c
	cc -std=c99 -Wall main.c token.c lexer.c -ledit -o repl
