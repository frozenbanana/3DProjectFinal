LIBS= -lGLEW -lSOIL -lassimp -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi
CC= g++
CFLAGS = -g -Wall -std=c++1z

make: Display.o Shader.o Model.o Mesh.o Camera.o LightHandler.o Light.o PntLight.o DirLight.o SptLight.o GBuffer.o Terrain.o QuadTree.o PingPongBuffer.o
		$(CC) $(CFLAGS) main.cpp -o run $^ $(LIBS)

Display.o: Display.cpp
		$(CC) $(CFLAGS) Display.cpp -c

Shader.o: Shader.cpp
		$(CC) $(CFLAGS) Shader.cpp -c

Model.o: Model.cpp
		$(CC) $(CFLAGS) Model.cpp -c

Mesh.o: Mesh.cpp
		$(CC) $(CFLAGS) Mesh.cpp -c

Camera.o: Camera.cpp
		$(CC) $(CFLAGS) Camera.cpp -c

LightHandler.o: LightHandler.cpp
		$(CC) $(CFLAGS) LightHandler.cpp -c

Light.o: Light.cpp
		$(CC) $(CFLAGS) Light.cpp -c

PntLight.o: PntLight.cpp
		$(CC) $(CFLAGS) PntLight.cpp -c

DirLight.o: DirLight.cpp
		$(CC) $(CFLAGS) DirLight.cpp -c

SptLight.o: SptLight.cpp
		$(CC) $(CFLAGS) SptLight.cpp -c

GBuffer.o: GBuffer.cpp
		$(CC) $(CFLAGS) GBuffer.cpp -c

Terrain.o: Terrain.cpp
		$(CC) $(CFLAGS) Terrain.cpp -c

QuadTree.o: QuadTree.cpp
		$(CC) $(CFLAGS) QuadTree.cpp -c

PingPongBuffer.o: PingPongBuffer.cpp
		$(CC) $(CFLAGS) PingPongBuffer.cpp -c
clean:
		-rm *.o
		-rm run
