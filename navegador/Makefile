# Define compiler and flags
CC = gcc
CFLAGS = -Wall -g
LDFLAGS =

# Define project name and source files
TARGET = ../boli_navegador # ../ para colocar na pasta anterior
SRCS = main.c http_client.c arquivo.c conexao.c # adicionar outros modulos se tiver
OBJS = $(SRCS:.c=.o) # Automatically generate object file names

# Default target: build the program
all: $(TARGET)

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

# Rule to compile each C source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Phony target for cleaning generated files
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)