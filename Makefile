CC = gcc
CFLAGS = -I./include
EXEC = simulation.exe
OBJ = main_sim.o consigne.o regulation.o visualisationT.o visualisationC.o simulateur.o

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXEC)