libraries := -ld2d1 -luser32 -lkernel32 -lwindowscodecs -lole32 -luuid
linker_flags := -Wl,-estart -Wl,--enable-stdcall-fixup -mconsole -nostdlib
flags := -Wall -Wextra -std=c89 -Wold-style-definition -nostdlib -ffreestanding -pedantic

main: main.o graphics.o ioutil.o entity.o
	gcc $^ -o $@ $(flags) $(linker_flags) $(libraries)

#LinkedList
containers/linkedlist_test: containers/linkedlist.o containers/linkedlist_test.o ioutil.o
	gcc $^ -o containers/linkedlist_test.exe $(flags) $(linker_flags) -lkernel32 -luser32

containers/linkedlist.o: containers/linkedlist.c containers/linkedlist.h
	gcc -g -c $< -o $@ $(flags)

containers/linkedlist_test.o: containers/linkedlist_test.c containers/linkedlist.c containers/linkedlist.h
	gcc -g -c $< -o $@ $(flags)

#HashMap
containers/hashmap_test: containers/hashmap.o containers/hashmap_test.o ioutil.o
	gcc $^ -o containers/hashmap_test.exe $(flags) $(linker_flags) -lkernel32 -luser32

containers/hashmap.o: containers/hashmap.c containers/hashmap.h
	gcc -g -c $< -o $@ $(flags)

containers/hashmap_test.o: containers/hashmap_test.c containers/hashmap.c containers/hashmap.h
	gcc -g -c $< -o $@ $(flags)

#Main
main.o: main.c gamestate.h ioutil.o graphics.o entity.o
	gcc -g -c $< $(flags)

graphics.o: graphics.c graphics.h ioutil.o
	gcc -g -c $< $(flags)

ioutil.o: ioutil.c ioutil.h
	gcc -g -c $< $(flags)

entity.o: entity.c entity.h ioutil.o
	gcc -g -c $< $(flags)

clean:
	rm -rf main.o graphics.o ioutil.o entity.o main.exe containers/linkedlist.o containers/linkedlist_test.o containers/linkedlist_test.exe

.PHONY: clean