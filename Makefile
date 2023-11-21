CC = gcc
CFLAGS = -Wall -g

banker: banker.c
	$(CC) $(CFLAGS) -o banker banker.c

clean:
	rm -f banker

