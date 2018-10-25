FILES_C = main.c red-black-tree.c linked-list.c

TARGET = main

LFLAGS = -lm

FILES_O =$(subst .c,.o,$(FILES_C))

$(TARGET): $(FILES_O) makefile
	gcc $(FILES_O) -o $(TARGET) $(LFLAGS)

all: $(TARGET)

run: ./$(TARGET)
	./$(TARGET) aeroports.csv dades.csv DEN

clean:
	/bin/rm $(FILES_O) $(TARGET)
