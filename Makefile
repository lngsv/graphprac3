CC=g++
LD=g++
CFLAGS=-c -g -std=gnu++17 -Wall -Wextra
LFLAGS=-lGL -lglfw -lGLEW -lpthread

OBJS=main.o \
	 Shader.o \
	 Texture.o \
	 Mesh.o \

TARGET=main


all: $(TARGET)

clean:
	rm -f $(OBJS) main

$(TARGET): $(OBJS)
	$(LD) $^ -o $@ $(LFLAGS)

%.o: %.cpp shaders/*
	$(CC) $(CFLAGS) $< -o $@
