build:
	gcc -o tema1 main.c FunctiiHash.c FunctiiLista.c
run: build
	./tema1
clean:
	rm -rf tema1 
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./tema1
