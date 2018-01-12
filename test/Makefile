CC = gcc
CFLAGS = -Wall -I../librk4 -I..

default:
	$(CC) $(CFLAGS) -c ../librk4/librk4.c -o librk4.o
	$(CC) $(CFLAGS) -c ../libhybrid.c -o libhybrid.o
	$(CC) $(CFLAGS) -c test_simple_hybrid.c -o test_simple_hybrid.o
	$(CC) librk4.o libhybrid.o test_simple_hybrid.o -o test.bin
