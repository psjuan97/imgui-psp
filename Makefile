TARGET = ImGUI
OBJS += imgui/imgui.o   imgui/imgui_draw.o imgui/imgui_tables.o  imgui/imgui_widgets.o
OBJS += imgui/backends/imgui_impl_opengl2.o
OBJS += imgui/backends/imgui_impl_glut.o
OBJS += myglut/glut.o

OBJS  += main.o 

INCDIR =
CFLAGS = -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti 
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS=  -lGL -lGLU  -lm -lc -lpsputility -lpspdebug -lpspge -lpspdisplay -lpspctrl -lpspsdk -lpspvfpu  -lpsppower -lstdc++

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Cube Sample

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
