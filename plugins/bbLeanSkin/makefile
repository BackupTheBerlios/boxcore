# --------------------------------------------------------------------
# makefile for bbLeanSkin - gcc mingw

OBJ1    = bbLeanSkin.o
BIN1    = bbLeanSkin.dll

OBJ2    = engine/hookctl.o engine/subclass.o
BIN2    = bbLeanSkinEng.dll

# INSTDIR = c:\bblean\plugins\bbLeanSkin

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
	rm -f $(OBJ1) $(OBJ2) $(BIN1) $(BIN2)

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
CC      = x86_64-pc-mingw32-gcc
CP      = x86_64-pc-mingw32-g++
WINDRES = x86_64-pc-mingw32-windres -J rc -O coff --include-dir $(<D)
DLLTOOL = x86_64-pc-mingw32-dlltool
endif

CFLAGS  = -Os -Wall -I../../blackbox
LIBS    = -lkernel32 -luser32 -lgdi32 -lstdc++ ../../blackbox/libBlackbox.a

# --------------------------------------------------------------------
$(BIN1): $(OBJ1) #makefile-gcc
	$(CC) -shared -s -o $(BIN1) $(OBJ1) $(LIBS)
	ls -al $(BIN1)

# For some reason the gcc 3.4 (rt 3.7) hangs in it's _DllMainCRTStartup
# That's why it is bypassed here with '-Wl,-e,_DllMain@12'
# It's likely a thread problem, does not happen with neither MSC
# nor BCC nor GCC 2.95

$(BIN2): $(OBJ2) makefile
	$(CC) -shared -s -o $(BIN2) $(OBJ2) $(LIBS) -Wl,-e,_DllMain@12
	ls -al $(BIN2)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------------------------
