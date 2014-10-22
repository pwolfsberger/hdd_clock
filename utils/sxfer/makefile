#CPP= cl
CPP=g++

COPTS=
ifeq ($(CPP), cl)
COPTS+= /I.
COPTS+= /I"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include"
COPTS+= /EHcs
else
COPTS=
COPTS+=-I.
COPTS+=-Wall
COPTS+=-Wextra 
endif

.PHONY: all clean
all: sxfer.exe

clean:
	del sxfer.exe

sxfer.exe: sxfer.cpp
	$(CPP) $(COPTS) $< -o sxfer

