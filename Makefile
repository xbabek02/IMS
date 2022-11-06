CC = g++
CFLAFS = -WALL -g

OBJ = main.o grid.o

all: ims

ims: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

	
##### objektové soubory #####

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c -o $@ $<