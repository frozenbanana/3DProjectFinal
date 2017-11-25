LIBS= -lGLEW -lSOIL -lassimp -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi
CC= g++
CFLAGS = -g -Wall

make: Display.o Shader.o Model.o Mesh.o Camera.o
		$(CC) $(CFLAGS) main.cpp -o run $^ $(LIBS)

Display.o: Display.cpp
		$(CC) $(CFLAGS) Display.cpp -c

Shader.o: Shader.cpp
		$(CC) $(CFLAGS) Shader.cpp -c

Model.o: Model.cpp
		$(CC) $(CFLAGS) Model.cpp -c

Mesh.o: Mesh.cpp
		$(CC) $(CFLAGS) Mesh.cpp -m

Camera.o: Camera.cpp
		$(CC) $(CFLAGS) Camera.cpp -c
