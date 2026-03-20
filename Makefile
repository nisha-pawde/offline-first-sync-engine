
all:
	gcc -Iinclude src/ipc.c src/local_node.c -o local_node -lpthread
	gcc -Iinclude src/sync_manager.c -o sync_manager
	gcc -Iinclude src/ipc.c src/central_server.c -o central_server
	gcc -Iinclude src/ipc.c src/logger.c -o logger
	gcc -Iinclude src/supervisor.c -o supervisor

run:
	./supervisor

clean:
	rm -f local_node sync_manager central_server logger supervisor
