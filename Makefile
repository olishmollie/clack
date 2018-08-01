all: clack lextest parsetest

clack: main.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c src/rational.c
	cc main.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c src/rational.c -ledit -o bin/clack

lextest: test/tokenizer.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c src/rational.c
	cc test/tokenizer.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c src/rational.c -o test/bin/tokenizer

parsetest: test/parser.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c src/rational.c
	cc test/parser.c src/tokenizer.c src/error.c src/token.c src/parser.c src/evaluator.c src/symtable.c src/rational.c -o test/bin/parser
