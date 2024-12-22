includes = includes
lib = lib
out = app/physforfun.exe
c_files = src/main.c src/point.c src/vector.c

all : $(OBJS)
#-Wl,-subsystem,windows
	gcc $(c_files) -I$(includes) -L$(lib) -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o $(out)