CFLAGS := -O2 -Wall -Wextra

OBJ := main.o

main: $(OBJ)

.PHONY: clean
clean:
	rm -f $(OBJ) main
