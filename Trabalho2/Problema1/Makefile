NOME_PROJETO := t2_p1
CC := gcc
C_FLAGS := -c
LINK_FLAGS := -pthread

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
	$(CC) $(C_FLAGS) $< -o $@

./bin/%.o: ./src/%.c ./headers/%.h ./headers/utils.h
	$(CC) $(C_FLAGS) $< -o $@

run: $(NOME_PROJETO)
	./$(NOME_PROJETO)

clean:
	rm -f ./bin/*.o $(NOME_PROJETO)