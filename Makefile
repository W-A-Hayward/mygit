# Variables
CC = gcc
CFLAGS = -Wall -Wextra 
SRC = main.c git_write.c hash_object.c
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

# Nettoyage
clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean

