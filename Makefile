# CPP=g++
CPP=clang++
INCLUDE=-I/usr/local/boost -I/usr/local/include -I.
FILES=app.cpp sim/*.cpp sim/io/*.cpp sim/config/*.c
# CFLAGS=/usr/local/lib/libgsl.a -O3 -Wall -isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5 
CFLAGS=/usr/local/lib/libgsl.a -O3 -Wall

all:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(FILES) $(CFLAGS) -o app
