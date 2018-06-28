all: fig lextest parsetest

fig: main.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c
	cc main.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c -ledit -o bin/fig

lextest: test/tokenizer.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c
	cc test/tokenizer.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c -o test/bin/tokenizer

parsetest: test/parser.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c
	cc test/parser.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c -o test/bin/parser
