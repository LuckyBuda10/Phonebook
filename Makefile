CC = gcc
CFLAGS = -Wall -pedantic

phonebook.out: phonebook.c
	$(CC) $(CFLAGS)  $^ -o $@

clean:
	rm -f phonebook.out
