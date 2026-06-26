CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2

SRCS = main.c registro.c arquivo.c abp_idx.c hash_idx.c avl_preco.c
OBJS = $(SRCS:.c=.o)
EXE = trabalho3

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(OBJS) $(EXE) produtos.bin
