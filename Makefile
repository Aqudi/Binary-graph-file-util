CC=g++
CPPFLAGS=-Wall -I./include
LDFLAGS=-L./lib

write: write 
read: read
partition: partition

test_write:
	make write && write.exe test.txt test.bin

test_read:
	make read && read.exe test.bin

test_partition:
	make partition && partition.exe test.bin 10

clean:
	del *.exe