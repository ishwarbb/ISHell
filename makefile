CC=gcc

all:
	@$(CC) builtin.c  discover.c execute.c fg-bg.c helperfunctions.c history.c init.c jobs.c ls.c main.c pinfo.c processControl.c prompt.c redirection.c signal.c -o ish -g 