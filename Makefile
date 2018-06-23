all: fig test

fig: main.c src/tokenizer.c src/error.c src/token.c
	cc main.c src/tokenizer.c src/error.c src/token.c -ledit -o bin/fig

test: test/tokenizer.c src/tokenizer.c src/error.c src/token.c
	cc test/tokenizer.c src/tokenizer.c src/error.c src/token.c -o test/bin/tokenizer

lextest:
	./test/bin/tokenizer
