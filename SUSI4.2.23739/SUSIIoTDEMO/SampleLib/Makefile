TARGET=libSampleLib.so

CFLAGS=-O2 -Wall -Werror -D _LINUX
CFLAGS+=-I../demo -I../IoTJsonLibrary
LINKS=../IoTJsonLibrary/output/libIoTJsonLibrary.a

output/$(TARGET): SampleLib.c SampleLib.h
	@mkdir -p $(dir $@)
	$(CC) -fPIC -shared -o $@ $< $(CFLAGS) $(LINKS)

release:
	mkdir -p $(RELEASE_PATH)
	cp -af *.c *.h $(RELEASE_PATH)
	cp -af Makefile_release $(RELEASE_PATH)/Makefile

clean:
	rm -rf output/
