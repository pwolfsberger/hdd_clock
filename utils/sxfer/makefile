CPP= cl

COPTS=
COPTS+= /I.
COPTS+= /I"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include"
COPTS+= /EHcs

.PHONY: all clean
all: sxfer.exe

clean:
	del sxfer.exe

sxfer.exe: sxfer.cpp
	$(CPP) $(COPTS) sxfer.cpp

