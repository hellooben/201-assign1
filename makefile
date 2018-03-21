OBJS = integer.o real.o string.o sll.o dll.o stack.o queue.o heap.o a.out bst.o sll-*-*.o dll-*-*.o stack-*-*.o queue-*-*.o heap-*-*.o scanner.o heapsort.o
LOPTS = -Wall -Wextra -std=c99
EXTRAS = integer.c real.c string.c sll.c dll.c stack.c queue.c
OEXTRAS = integer.o real.o string.o sll.o dll.o stack.o queue.o

all : test-sll test-dll test-stack test-queue test-bst

test-sll :
	gcc $(LOPTS) -c sll.c integer.c tests/sll-0-0.c
	gcc $(LOPTS) sll-0-0.o integer.o sll.o -o test-sll

test-dll :
	gcc $(LOPTS) -c dll.c integer.c tests/dll-0-0.c
	gcc $(LOPTS) dll-0-0.o integer.o dll.o -o test-dll

test-stack :
	gcc $(LOPTS) -c stack.c dll.c integer.c tests/stack-0-0.c
	gcc $(LOPTS) stack-0-0.o integer.o dll.o stack.o -o test-stack

test-queue :
	gcc $(LOPTS) -c queue.c sll.c integer.c tests/queue-0-0.c
	gcc $(LOPTS) queue-0-0.o integer.o sll.o queue.o -o test-queue

test-bst :
	gcc $(LOPTS) -c bst.c $(EXTRAS) tests/bst-2-10.c
	gcc $(LOPTS) bst-2-10.o $(OEXTRAS) bst.o -o test-bst

test-heap :
	gcc $(LOPTS) -c heap.c bst.c $(EXTRAS) tests/heap-2-12.c
	gcc $(LOPTS) heap-2-12.o $(OEXTRAS) bst.o heap.o -o test-heap

test-heapsort :
	gcc $(LOPTS) -c heapsort.c heap.c bst.c scanner.c $(EXTRAS)
	gcc $(LOPTS) $(OEXTRAS) scanner.o bst.o heap.o heapsort.o -o heapsort

valgrind :
	echo testing singly-linked list
	valgrind ./test-sll
	echo
	echo testing doubly-linked list
	valgrind ./test-dll
	echo
	echo testing stack
	valgrind ./test-stack
	echo
	echo testing queue
	valgrind ./test-queue
	echo
	echo testing bst
	valgrind ./test-bst
	echo
	echo testing heap
	valgrind ./test-heap
	echo

test :
	# ./test-sll
	# ./test-dll
	# ./test-stack
	# ./test-queue
	# ./test-bst
	# ./test-heap
	./heapsort tests/hs-0-0.data

clean :
	rm -f $(OBJS) test-*.o bst-*-*.o test-bst test-sll test-dll test-stack test-queue test-heap
