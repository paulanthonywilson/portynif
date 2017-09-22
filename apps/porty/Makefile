CFLAGS= -g

HEADER_FILES = src
SOURCE_FILES =$(wildcard src/*.c)

OBJECT_FILES = $(SOURCE_FILES:.c=.o)

c_priv/echo: clean c_priv $(OBJECT_FILES)
	mkdir -p c_priv
	$(CC) -I $(HEADER_FILES) -o $@ $(LDFLAGS) $(OBJECT_FILES) $(LDLIBS)

c_priv:
	mkdir -p c_priv

clean:
	rm -f c_priv/echo $(OBJECT_FILES) $(BEAM_FILES)
