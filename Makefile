game_libraries := -ld2d1 -luser32 -lkernel32 -lwindowscodecs -lole32 -luuid
server_libraries := -lws2_32 -lkernel32 -luser32

linker_flags := -Wl,-estart -Wl,--enable-stdcall-fixup -mconsole -nostdlib
flags := -Wall -Wextra -std=c99 -Wold-style-definition -nostdlib -ffreestanding -pedantic -Werror

main: obj/main.o obj/graphics.o obj/ioutil.o obj/entity.o obj/gamestate.o obj/chkstk.o
	gcc $^ -o $@ $(flags) $(linker_flags) $(game_libraries)

obj/chkstk.o: chkstk.S
	gcc -c $< -o $@

#LinkedList
containers/linkedlist_test: containers/linkedlist.o containers/linkedlist_test.o obj/ioutil.o
	gcc $^ -o containers/linkedlist_test.exe $(flags) $(linker_flags) -lkernel32 -luser32

obj/containers/linkedlist.o: containers/linkedlist.c containers/linkedlist.h
	gcc -g -c $< -o $@ $(flags)

obj/containers/linkedlist_test.o: containers/linkedlist_test.c containers/linkedlist.c containers/linkedlist.h
	gcc -g -c $< -o $@ $(flags)

#HashMap
containers/hashmap_test: containers/hashmap.o containers/hashmap_test.o obj/ioutil.o obj/chkstk.o
	gcc $^ -o containers/hashmap_test.exe $(flags) $(linker_flags) -lkernel32 -luser32

obj/containers/hashmap.o: containers/hashmap.c containers/hashmap.h
	gcc -g -c $< -o $@ $(flags)

obj/containers/hashmap_test.o: containers/hashmap_test.c containers/hashmap.c containers/hashmap.h
	gcc -g -c $< -o $@ $(flags)

#Main
obj/main.o: main.c gamestate.h obj/ioutil.o obj/graphics.o obj/entity.o obj/gamestate.o
	gcc -g -c $< -o $@ $(flags)

obj/graphics.o: graphics.c graphics.h obj/ioutil.o
	gcc -g -c $< -o $@ $(flags)

obj/ioutil.o: ioutil.c ioutil.h
	gcc -g -c $< -o $@ $(flags)

obj/entity.o: entity.c entity.h obj/ioutil.o
	gcc -g -c $< -o $@ $(flags)

obj/gamestate.o: gamestate.c gamestate.h obj/graphics.o obj/ioutil.o
	gcc -g -c $< -o $@ $(flags)

#Server
server: obj/server/main.o obj/ioutil.o
	gcc $^ -o $@ $(flags) $(linker_flags) $(server_libraries)

obj/server/main.o: server/main.c
	gcc -g -c $< -o $@ $(flags)

clean:
	rm -rf obj/*.o obj/server/*.o *.exe

.PHONY: clean

