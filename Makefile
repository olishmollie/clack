repl: main.c src/lexer.c src/parser.c src/token.c src/ast.c src/interpreter.c
	cc -std=c99 -Wall main.c src/lexer.c src/parser.c src/token.c src/ast.c src/interpreter.c -ledit -o fig
