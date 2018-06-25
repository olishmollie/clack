all: fig test

fig: main.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c
	cc main.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c -ledit -o bin/fig

test: test/tokenizer.c src/tokenizer.c src/error.c src/token.c
	cc test/tokenizer.c src/tokenizer.c src/error.c src/token.c -o test/bin/tokenizer

lextest:
	./test/bin/tokenizer
