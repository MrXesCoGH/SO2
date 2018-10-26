FILES_C = main.c red-black-tree.c linked-list.c

TARGET = main

LFLAGS = -lm

CFLAGS = -Wall -g

FILES_O =$(subst .c,.o,$(FILES_C))

$(TARGET): $(FILES_O) makefile
	gcc -g $(FILES_O) -o $(TARGET) $(LFLAGS)

all: $(TARGET)

run: ./$(TARGET)
	./$(TARGET)
	
%.o: %.c Makefile
	gcc $(CFLAGS) -c $<

clean:
	/bin/rm $(FILES_O) $(TARGET)
