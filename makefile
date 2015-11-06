
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Wno-deprecated -Werror -Wstack-usage=1024 -o

masterMind: masterMind.c
	gcc $(CFLAGS) masterMind masterMind.c
