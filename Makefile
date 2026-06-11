# Variables
CC = gcc
CFLAGS = -Wall -Wextra 
SRC = main.c git_write.c hash_object.c write_tree.c arena.c init.c
OBJ = $(SRC:.c=.o)
EXEC = mygit

# Règle par défaut
all: $(EXEC)

# Création de l'exécutable (ajout de -lz pour lier zlib)
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -lcrypto -lz

# Règle générique pour les fichiers .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage (suppression des objets, de l'exécutable et de .mygit)
clean:
	rm -f $(OBJ) $(EXEC) .mygit

.PHONY: all clean

