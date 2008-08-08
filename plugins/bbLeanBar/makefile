# --------------------------------------------------------------------
# makefile for bbLeanBar - gcc (mingw)

# Compile:
#   you may want to change INST below
#   make -f makefile-gcc

# Versions:
#   gcc-3.4.2
#   binutils-2.15.90
#   mingw-runtime-3.7
#   w32api-3.2

OBJ     = bbLeanBar.o ../BBPlugin/BBPlugin.o 
RES     =
BIN     = bbLeanBar.dll
# INST    = c:\bblean\plugins\bbLeanBar

BBAPI   = ../../blackbox
BBLIB   = $(BBAPI)/libBlackbox.a

# Uncomment the next line or use "make ENABLEEXTENDERS=1" to build with extenders
# ENABLEEXTENDERS = 1

ifdef ENABLEEXTENDERS
DEFS = -DWITH_EXTENDERS
endif


ifdef INSTDIR
all: $(INST)/$(BIN)
else
all: $(BIN)
endif

bbLeanBar.o: \
	bbLeanBar.cpp \
	bbLeanClasses.cpp \
	Utils.cpp \
	Tooltips.cpp \
	DrawIco.cpp \
	BuffBmp.cpp \
	SysMenu.cpp \
	TinyDropTarg.cpp \
	../../blackbox/Tinylist.cpp \
	bartender.cpp

clean:
	rm -f $(OBJ) $(RES) $(BIN)

$(INST)/$(BIN): $(BIN)
	copy $(BIN) $(INST)

$(BBLIB): $(BBAPI)/libBlackbox.def
	dlltool -d $< -l $@ -D blackbox.exe

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

CFLAGS  = -c -Wall -Os -I$(BBAPI)\
	-fno-rtti \
	-fno-exceptions \
	-fomit-frame-pointer \
	$(DEFS)

LIBS = \
	-lshell32 -luser32 -lkernel32 -lgdi32 \
	-lcomctl32 -lole32 -luuid \
	-lstdc++

$(BIN): $(OBJ) $(BBLIB) makefile
	$(CC) -mwindows -shared -s -o $(BIN) $(OBJ) $(LIBS) $(BBLIB)
	ls -al $(BIN)


# --------------------------------------------------------------------

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

%.res: %.rc
	$(WINDRES) -i $< -o $@

# --------------------------------------------------------------------

