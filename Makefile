libraries := -ld2d1 -luser32 -lkernel32 -lwindowscodecs -lole32 -luuid
linker_flags := -Wl,-estart -Wl,--enable-stdcall-fixup -mconsole -nostdlib -ffreestanding
flags := -Wall -Wextra -std=c89 -pedantic -Wold-style-definition

main: main.o graphics.o error.o
	gcc $^ -o $@ $(flags) $(linker_flags) $(libraries)

main.o: main.c
	gcc -g -c $< $(flags)

graphics.o: graphics.c graphics.h
	gcc -g -c $< $(flags)

error.o: error.c error.h
	gcc -g -c $< $(flags)

clean:
	rm -rf main.o graphics.o error.o main.exe

.PHONY: clean