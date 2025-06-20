$(info ***** SusiIoT demo (Linux) *****)

TARGET=SusiiotDemo

CXXFLAGS = -Wall -O2 -I. -D _LINUX
LINKS = ../../library/libjansson.so.4 ../../library/libSusiIoT.so

$(TARGET): main.cpp common.h
	@mkdir -p output
	$(CXX) $(CXXFLAGS) main.cpp -o output/$@ $(LINKS)

RELEASE:
	@mkdir -p demo
	cp *.h *.cpp *.json Makefile_release demo
	cp ../SusiIoT/OsDeclarations.h .
	cp ../SusiIoT/SusiIoTAPI.h .
	cp ../SusiIoT/SusiIoT.h .

clean:
	rm -rf output/ *.vcproj
