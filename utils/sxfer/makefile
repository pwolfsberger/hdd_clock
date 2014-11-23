PROJ=picpgm

#CPP= cl
CPP=g++

DEFINE=

COPTS=
ifeq ($(CPP), cl)
COPTS+= /I.
COPTS+= /I"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include"
COPTS+= /EHcs
else
COPTS+=-I.
COPTS+=-Wall
COPTS+=-Wextra 
COPTS+=-std=c++11
endif

LFLAGS=
VPATH=

include srcs.mk
SRCS_OBJS:=$(patsubst %.cpp,%.o,$(SRCS))
# In case there are mixed sources (c/c++)
SRCS_OBJS:=$(patsubst %.c,%.o,$(SRCS_OBJS))

.PHONY: all clean
all: $(PROJ).exe

$(PROJ).exe: $(SRCS_OBJS)
	$(CPP) -o $(PROJ) $(LFLAGS) $^

define SRC_TARGET
$(basename $(1)).o: $(1)  
	$(CPP) $(COPTS) $(DEFINE) -c -o $$@ $$<
endef
$(foreach src,$(SRCS),$(eval $(call SRC_TARGET,$(src))))

GENERATED_FILES=
GENERATED_FILES+=$(SRCS_OBJS)
GENERATED_FILES+=$(PROJ).exe

GENERATED_FILES_CLEAN:=$(patsubst %,%_clean,$(GENERATED_FILES))

clean: $(GENERATED_FILES_CLEAN)

.PHONY: $(GENERATED_FILES_CLEAN)
$(GENERATED_FILES_CLEAN):
	rm -f $(patsubst %_clean,%,$@)

