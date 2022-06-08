TARGET = FlappyBirdPSP
OBJS = src/main.c \
       src/mutil.c \
	   src/psp/triInput.o \
	   src/psp/triAudioLib.o \
	   src/psp/triMemory.o \
	   src/psp/triRefcount.o \
	   src/psp/triWav.o \
       src/battle/battle.c \
       src/chars/dtsans1.c \
	   src/glib2d.o 

LIBS = -ljpeg -lpng -lz -lpspgum -lpspgu -lpsprtc -lm -lpspvram -lpspaudio -lpspmp3

CFLAGS = -O2 -G0 -Wall -g -fno-builtin-printf
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

# PSP JUNK
PSP_FW_VERSION = 500
PSP_LARGE_MEMORY = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = DT PSP
PSP_EBOOT_ICON = ICON0.PNG
PSP_EBOOT_PIC1 = PIC1.PNG


PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak 
