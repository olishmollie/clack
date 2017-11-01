repl: main.c src/lexer.c src/parser.c
	cc -std=c99 -Wall main.c src/lexer.c src/parser.c -ledit -o repl
