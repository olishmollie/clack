repl: main.c src/lexer.c src/parser.c src/emitter.c src/error.c
	cc main.c src/lexer.c src/parser.c src/emitter.c src/error.c -o fig
