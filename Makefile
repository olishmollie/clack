repl: main.c istream.c
	cc -std=c99 -Wall main.c istream.c -ledit -o repl
