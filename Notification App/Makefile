all: server subscriber

common.o: utils/common.c
	gcc -w -c -g utils/common.c -o common.o

utils_server.o: utils/utils_server.c
	gcc -w -c -g utils/utils_server.c -o utils_server.o

utils_subscriber.o: utils/utils_subscriber.c
	gcc -w -c -g utils/utils_subscriber.c -o utils_subscriber.o

# Compile server
server: server.c common.o utils_server.o
	gcc -w -g server.c common.o utils_server.o -o server

# Compile subscriber
subscriber: subscriber.c common.o utils_subscriber.o
	gcc -w -g subscriber.c common.o utils_subscriber.o -o subscriber

clean:
	rm -rf server subscriber *.o
