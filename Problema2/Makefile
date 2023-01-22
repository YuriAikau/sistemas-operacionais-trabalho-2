NOME_PROJETO := t2_p2
CC := gcc
C_FLAGS := -c
LINK_FLAGS := -pthread
INCLUDE := headers

# arquivos .h
H_SRC = $(wildcard ./headers/*.h)

# arquivos .c
C_SRC = $(wildcard ./src/*.c)

# arquivos objeto (.o)
OBJ = $(C_SRC:./src/%.c=./bin/%.o)

all: $(NOME_PROJETO)

$(NOME_PROJETO): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LINK_FLAGS)

./bin/main.o: ./src/main.c ./headers/utils.h
	$(CC) $(C_FLAGS) $< -o $@ -I $(INCLUDE)

./bin/%.o: ./src/%.c ./headers/%.h ./headers/utils.h
	$(CC) $(C_FLAGS) $< -o $@ -I $(INCLUDE)

run: $(NOME_PROJETO)
	./$(NOME_PROJETO)

%.txt: $(NOME_PROJETO)
	./$(NOME_PROJETO) > $@

clean:
	rm -f ./bin/*.o *.txt $(NOME_PROJETO)