CC=gcc #compiler
SOURCE1=src/lexi.c #sources to compile
SOURCE2=src/token.c
SOURCE3=src/symbol.c
SOURCE4=src/parser.c
SOURCE5=src/pm0.c
SOURCE6=src/err.c
SOURCE7=src/main.c
TARGET=hw4compiler #target program name

hw4compiler:
	$(CC) $(SOURCE1) $(SOURCE2) $(SOURCE3) $(SOURCE4) $(SOURCE5) $(SOURCE6) $(SOURCE7) -o $(TARGET)

clean:
	rm $(TARGET)