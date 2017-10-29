repl: main.c src/lexer.c src/interpreter.c
	cc -std=c99 -Wall main.c src/lexer.c src/interpreter.c -ledit -o repl
