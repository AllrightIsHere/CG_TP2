CC=gcc
OPCOES_COMPILADOR=-I. -I/../SOIL -lglut -lGLU -lGLEW -lGL -lSOIL -lSDLmain -lSDL_mixer -lSDL -lm 
TODOS_ARQUIVOS_PONTOH =
TODOS_ARQUIVOS_OBJ = main.o glm.o glmimg.o glm_util.o coordenadas.o

install:
	sudo apt-get install build-essential mesa-common-dev g++ freeglut3-dev libglew-dev libsoil-dev libsdl1.2-dev libsdl-mixer1.2-dev

%.o: ../Sources/%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o main $^ $(OPCOES_COMPILADOR)

run: all
	./main

clean:
	rm *.o main
