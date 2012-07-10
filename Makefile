all: gpc

gpc: main.o parser.o ast.o tokenizer.o
	g++ main.o parser.o ast.o tokenizer.o -o gpc

main.o: main.cpp parser.hpp ast.hpp tokenizer.hpp
	g++ -Wall -pedantic -c main.cpp -o main.o

parser.o: parser.cpp ast.hpp tokenizer.hpp
	g++ -Wall -pedantic -c parser.cpp -o parser.o

ast.o: ast.cpp ast.hpp
	g++ -Wall -pedantic -c ast.cpp -o ast.o

tokenizer.o: tokenizer.cpp tokenizer.hpp
	g++ -Wall -pedantic -c tokenizer.cpp -o tokenizer.o

.PHONY : clean
clean:
	rm -f *.o gpc
