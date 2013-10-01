LIBNAME = lpeg
LUADIR=../LuaJIT-2.0.2/src
LUALIB=../LuaJIT-2.0.2/src

ifeq ($(OS),Windows_NT)
DLLFLAGS = -shared
OUTPUT = lpeg.dll

else ifeq (Darwin,$(shell uname -s))
DLLFLAGS = -bundle -undefined dynamic_lookup
OUTPUT = lpeg.so

else
DLLFLAGS = -shared
OUTPUT = lpeg.so
endif

COPT = -O2

CWARNS = -Wall -Wextra -pedantic \
	-Waggregate-return \
	-Wcast-align \
	-Wcast-qual \
	-Wdisabled-optimization \
	-Wpointer-arith \
	-Wshadow \
	-Wsign-compare \
	-Wundef \
	-Wwrite-strings \
	-Wbad-function-cast \
	-Wdeclaration-after-statement \
	-Wmissing-prototypes \
	-Wnested-externs \
	-Wstrict-prototypes \

CFLAGS = $(CWARNS) $(COPT) -ansi -I$(LUADIR)
CC = gcc

FILES = lpvm.o lpcap.o lptree.o lpcode.o lpprint.o


all: $(FILES)
	$(CC) -L$(LUALIB) -llua51 $(DLLFLAGS) $(FILES) -o $(OUTPUT)

$(FILES): makefile

test: test.lua re.lua lpeg.so
	./test.lua

clean:
	rm -f $(FILES) lpeg.so


lpcap.o: lpcap.c lpcap.h lptypes.h
lpcode.o: lpcode.c lptypes.h lpcode.h lptree.h lpvm.h lpcap.h
lpprint.o: lpprint.c lptypes.h lpprint.h lptree.h lpvm.h lpcap.h
lptree.o: lptree.c lptypes.h lpcap.h lpcode.h lptree.h lpvm.h lpprint.h
lpvm.o: lpvm.c lpcap.h lptypes.h lpvm.h lpprint.h lptree.h

