EXEC = renderer

PLATFORM := x64
BITS := 64d

CC = g++
CXX = g++
CFLAGS += -g -O3 -DLINUX -Wall -Wextra

LFLAGS += -L/usr/X11R6/lib -lX11
INCLUDES = -I. -Iinclude -Iglm -I/usr/X11R6/include

OBJS = src/main.o src/math.o src/modelling/triangle.o src/modelling/mesh.o src/modelling/scene.o src/modelling/camera.o src/display/x11display.o src/rasterizer/rasterizer.o

$(EXEC): $(OBJS)
	$(CXX) -o $(EXEC) $(OBJS) $(CFLAGS) $(LIBS) $(LFLAGS)

all: $(OBJS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(INCLUDES) $< -o $@

clean::
	rm -f $(EXEC)
	find . -type f -name "*.o" -delete
