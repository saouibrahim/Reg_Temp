CC = gcc # compiler variable, to compile .c files to .o files
CFLAGS = -I./include #to say to the code compliation that when u are looking for the header its in the include folder


SHARED_OBJ = consigne.o regulation.o visualisationT.o visualisationC.o  #object files that are both needed for sim and usb executables

SIM_OBJ = main_sim.o simulateur.o $(SHARED_OBJ) # all object files needed for sim.exe

USB_OBJ = main_usb.o releve.o commande.o $(SHARED_OBJ) # idem


all: sim.exe usb.exe # the targets of our make file to build


sim.exe: $(SIM_OBJ) # the full command to build sim.exe from all the object files in SIM_OBJ
	$(CC) $(SIM_OBJ) -o sim.exe 

usb.exe: $(USB_OBJ) # idem
	$(CC) $(USB_OBJ) -o usb.exe -lftd2xx


%.o: %.c # a pattern rule that handles the compilation of .c files to .o files, first pre requisite is the .c file, the target is the .o, CFLAGS is teh -I./include
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o sim.exe usb.exe

# Removes all generated files so you can start fresh.
# rm -f   — force-delete, no error if files don't exist
# *.o     — every object file in the current directory
# as well as the executables