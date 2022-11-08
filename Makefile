CC = g++
CFLAGS = -Wall -g

OBJ = main.o grid.o simulation.o bomber.o fighter.o plane.o withid.o distance.o

all: ims

ims: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@


##### objektové soubory #####

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o ims