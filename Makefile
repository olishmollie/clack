all: fig test

fig: main.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c
	cc main.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c -ledit -o bin/fig

test: test/tokenizer.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c
	cc test/tokenizer.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c -o test/bin/tokenizer

lextest:
	./test/bin/tokenizer
