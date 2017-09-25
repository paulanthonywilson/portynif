CFLAGS= -g

HEADER_FILES = src

SRC =$(wildcard src/*.c)

OBJ = $(SRC:.c=.o)

DEFAULT_TARGETS ?= c_priv priv/c/echo

priv/c/echo: c_priv $(OBJ)
	$(CC) -I $(HEADER_FILES) -o $@ $(LDFLAGS) $(OBJ) $(LDLIBS)

c_priv:
	mkdir -p priv/c

clean:
	rm -f priv/c $(OBJ) $(BEAM_FILES)
