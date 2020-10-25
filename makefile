CC = gcc
all: SqUnitTest SqMerge
SqUnitTest: SqUnitTest.c SqList.o
	$(CC) SqUnitTest.c SqList.o -o SqUnitTest
SqMerge: SqMerge.c SqList.o
	$(CC) SqMerge.c SqList.o -o SqMerge

SqList.o:SqList.c
	$(CC) -c SqList.c
clean: 
	del *.o *.exe SqUnitTest SqMerge