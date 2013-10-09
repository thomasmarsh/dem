CC	= g++
CFLAGS	= -O3 -ffast-math -Wall
#LIBS	= -lGL -lGLU -lglut -lpthread
#LIBS	= -lopengl32 -lglu32 -lglut
LIBS    = -framework GLUT -framework OpenGL -framework Cocoa

.SUFFIXES: .cc .o


SRC = control.cc display.cc main.cc render.cc thread.cc
OBJ	= $(SRC:.cc=.o)
TARGET	= dem

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)


control.o: control.cc control.h common.h render.h rect.h thread.h \
  buffer.h generator.h formula.h display.h orbit.h views.h
display.o: display.cc orbit.h common.h display.h thread.h rect.h render.h \
  buffer.h generator.h formula.h control.h
main.o: main.cc control.h common.h render.h rect.h thread.h buffer.h \
  generator.h formula.h display.h orbit.h
render.o: render.cc render.h common.h rect.h thread.h buffer.h \
  generator.h formula.h control.h display.h orbit.h plane.h
thread.o: thread.cc thread.h
