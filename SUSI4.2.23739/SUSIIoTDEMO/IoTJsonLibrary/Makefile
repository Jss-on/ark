CFLAGS=-O2 -Wall -Werror
LINKS=../../library/libjansson.a

libIoTJsonLibrary.a: IoTJsonLibrary.o
	$(AR) rcs output/libIoTJsonLibrary.a output/IoTJsonLibrary.o $(LINKS)

IoTJsonLibrary.o : IoTJsonLibrary.c IoTJsonLibrary.h
	@mkdir -p output
	$(CC) -c $(CFLAGS) IoTJsonLibrary.c -o output/IoTJsonLibrary.o

clean:
	rm -rf output/
