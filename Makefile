

# Executables

FS_assg: src/assg.c src/spheres.c src/vector.c src/color.c
	gcc -Wall -Werror -std=c99 -DFS -o FS_assg src/assg.c src/spheres.c src/vector.c src/color.c -lm

clean:
	rm -f FS_assg

# Object files
# TODO: Makefile rules to compile source files into object files

