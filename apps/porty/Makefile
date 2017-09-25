CFLAGS= -g

HEADER_FILES = src
SOURCE_FILES =$(wildcard src/*.c)

OBJECT_FILES = $(SOURCE_FILES:.c=.o)

priv/c/echo: clean c_priv $(OBJECT_FILES)
	mkdir -p priv/c
	$(CC) -I $(HEADER_FILES) -o $@ $(LDFLAGS) $(OBJECT_FILES) $(LDLIBS)

c_priv:
	mkdir -p priv/c

clean:
	rm -f priv/c/echo $(OBJECT_FILES) $(BEAM_FILES)
