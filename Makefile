CC=gcc

DIRS=bin obj

SDIR=./src
BDIR=./bin

CFLAGS=-I$(IDIR) -ggdb
IDIR=./include

ODIR=./obj
_OBJ= main.o processor.o utils.o memory.o instructions.o display.o interrupts.o
OBJ= $(patsubst %, $(ODIR)/%, $(_OBJ))

$(shell mkdir -p $(DIRS))

$(BDIR)/gboy_emu: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lSDL2

$(ODIR)/main.o: $(SDIR)/main.c $(IDIR)/processor.h $(IDIR)/memory.h $(IDIR)/display.h $(IDIR)/interrupts.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/processor.o: $(SDIR)/processor.c $(IDIR)/processor.h $(IDIR)/utils.h $(IDIR)/instructions.h $(IDIR)/interrupts.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/interrupts.o: $(SDIR)/interrupts.c $(IDIR)/interrupts.h $(IDIR)/processor.h $(IDIR)/memory.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/display.o: $(SDIR)/display.c $(IDIR)/display.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/memory.o: $(SDIR)/memory.c $(IDIR)/memory.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/instructions.o: $(SDIR)/instructions.c $(IDIR)/instructions.h $(IDIR)/memory.h $(IDIR)/utils.h $(IDIR)/interrupts.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/utils.o: $(SDIR)/utils.c $(IDIR)/utils.h $(IDIR)/processor.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean, run

clean:
	@$(RM) $(OBJ)/*.o $(BDIR)/gboy_emu

run: $(BDIR)/gboy_emu
	@$(BDIR)/gboy_emu
