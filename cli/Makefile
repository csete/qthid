LIBS=`pkg-config libusb-1.0 --libs`
CFLAGS=`pkg-config libusb-1.0 --cflags` -I.. -Wall

SOURCES = \
	main.cpp \
	../fcd.c \
	../hid-libusb.c

qthid-cli: $(SOURCES)
	g++ $(CFLAGS) $(LIBS) -o qthid-cli $(SOURCES)
