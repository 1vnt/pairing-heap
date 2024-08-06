CFLAGS := -O3 -pg

OBJ := main.o

main: $(OBJ)

.PHONY: clean
clean:
	rm -f $(OBJ) main
