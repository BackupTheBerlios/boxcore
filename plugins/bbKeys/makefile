# --------------------------------------------------------------------
# makefile for BBKeys - gcc mingw

OBJ1    = BBKeys.o
BIN1    = BBKeys.dll

OBJ2    = winkeyhook.o
BIN2    = winkeyhook.dll

# INSTDIR = c:\bblean\plugins\BBKeys

# -------------------------------------------
# targets

ifdef INSTDIR
all: $(INSTDIR)\$(BIN1) $(INSTDIR)\$(BIN2)
else
all: $(BIN1) $(BIN2)
endif

# -------------------------------------------
$(INSTDIR)\$(BIN1): $(BIN1)
	copy $(BIN1) $(INSTDIR)

$(INSTDIR)\$(BIN2): $(BIN2)
	copy $(BIN2) $(INSTDIR)

clean:
	rm -f $(OBJ1) $(OBJ2) $(BIN1) $(BIN2) *.exp

# --------------------------------------------------------------------
ifndef VERSION
VERSION = native
endif

ifeq ($(VERSION),native)
CC      = gcc.exe
CP      = g++.exe
WINDRES = windres.exe -J rc -O coff --include-dir $(<D)
DLLTOOL = dlltool.exe
endif
ifeq ($(VERSION),cross_x64)
CC      = x86_64-pc-mingw32-gcc.exe
CP      = x86_64-pc-mingw32-g++.exe
WINDRES = x86_64-pc-mingw32-windres.exe -J rc -O coff --include-dir $(<D)
DLLTOOL = x86_64-pc-mingw32-dlltool.exe
endif

CFLAGS  = -Os -Wall -c -mwindows -I../../blackbox
LIBS    = -lshell32 -luser32 -lkernel32 -lgdi32 -lcomctl32 -lstdc++ ../../blackbox/libBlackbox.a

# --------------------------------------------------------------------
$(BIN1): $(OBJ1) makefile
	$(CC) -shared -s -o $(BIN1) $(OBJ1) $(LIBS)
	ls -al $(BIN1)

$(BIN2): $(OBJ2) makefile
	$(CC) -shared -s -o $(BIN2) $(OBJ2) $(LIBS)
	ls -al $(BIN2)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

# --------------------------------------------------------------------
