.PHONY: all clean

SRC = read_boot.c read_mbr.c read_root.c lecturaRoot.c fileRecover.c
BIN = $(SRC:.c=)

all: 
	gcc read_boot.c -o read_boot
	gcc read_mbr.c -o  read_mbr
	gcc read_root.c -o read_root
	gcc lecturaRoot.c -o lecturaRoot
	gcc fileRecover.c -o fileRecover
clean:
	rm -f $(BIN) $(OBJ)


