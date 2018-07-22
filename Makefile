CC=gcc

gboy_emu: main.o processor.o
	$(CC) $^ -o $@

main.o: main.c constants.h processor.h
	$(CC) -c $< -o $@

processor.o: processor.c processor.h
	$(CC) -c $< -o $@

.PHONY: clean, run

clean:
	@$(RM) *.o gboy_emu 

run:	gboy_emu
	@./gboy_emu
