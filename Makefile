CC=gcc

SDIR=./src
BDIR=./bin

CFLAGS=-I$(IDIR)
IDIR=./include

ODIR=./obj
_OBJ= main.o processor.o
OBJ= $(patsubst %, $(ODIR)/%, $(_OBJ)) 

$(BDIR)/gboy_emu: $(OBJ) 
	$(CC) $^ -o $@

$(ODIR)/main.o: $(SDIR)/main.c $(IDIR)/processor.h $(IDIR)/constants.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/processor.o: $(SDIR)/processor.c $(IDIR)/processor.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean, run

clean:
	@$(RM) $(OBJ)/*.o $(BDIR)/gboy_emu 

run: $(BDIR)/gboy_emu
	@$(BDIR)/gboy_emu
