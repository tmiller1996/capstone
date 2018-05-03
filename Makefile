CFLAGS=`pkg-config --cflags guile-2.2 bdw-gc sdl2 SDL2_image` -fPIC -Wall -Werror -O2
LDLIBS=`pkg-config --libs guile-2.2 bdw-gc sdl2 SDL2_image`
LDFLAGS=-O2

OBJECTS=core.o keytable.o input.o rgba.o rect.o point.o graphics.o sound.o

core.so: $(OBJECTS)
	$(CC) $(LDFLAGS) $(LDLIBS) -fPIC -shared -o core.so $(OBJECTS)
clean:
	$(RM) core.so $(OBJECTS)