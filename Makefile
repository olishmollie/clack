repl: main.c src/stack.c src/lexer.c src/parser.c src/evaluator.c src/error.c
	cc main.c src/stack.c src/lexer.c src/parser.c src/evaluator.c src/error.c -ledit -o bin/fig
