repl: main.c src/token.c src/lexer.c
	cc -std=c99 -Wall main.c src/token.c src/lexer.c -ledit -o repl
