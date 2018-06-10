repl: main.cpp src/lexer.cpp src/parser.cpp 
	g++ -std=c++14 -Wall main.cpp src/lexer.cpp src/parser.cpp -o fig
