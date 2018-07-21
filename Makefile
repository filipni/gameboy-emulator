CC=gcc

gboy_emu: main.o
	$(CC) $< -o $@

main.o: main.c constants.h
	$(CC) -c $< -o $@

.PHONY: clean, run

clean:
	$(RM) *.o gboy_emu 

run:	gboy_emu
	@./gboy_emu
