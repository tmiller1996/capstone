CFLAGS=`pkg-config --cflags guile-2.2 sdl2` -fPIC -Wall
LDLIBS=`pkg-config --libs guile-2.2 sdl2`

OBJECTS=core.o

core.so: $(OBJECTS)
	$(CC) $(LDFLAGS) $(LDLIBS) -fPIC -shared -o core.so $(OBJECTS)
clean:
	$(RM) core.so $(OBJECTS)