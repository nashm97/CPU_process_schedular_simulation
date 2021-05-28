allocate: allocate.c linked_list.c process.c processor.c
	gcc -o allocate allocate.c linked_list.c process.c processor.c -Wall
clean :
