main: gl_world_third_person.o
	gcc -Wall -o gl_world_third_person_x64 gl_world_third_person.o `pkg-config --libs x11 sdl gl` -lm

gl_world_third_person.o: gl_world_third_person.c
	gcc -Wall -c gl_world_third_person.c `pkg-config --cflags sdl`

clean:
	rm -f *.o gl_world_third_person_x64

run: gl_world_third_person_x64
	./gl_world_third_person_x64

