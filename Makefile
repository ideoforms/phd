# CPP=g++
CPP=clang++
INCLUDE=-I/usr/local/boost -I/usr/local/include -I.
SIM=app-utils.cpp sim/*.cpp sim/io/*.cpp sim/config/*.c
# CFLAGS=/usr/local/lib/libgsl.a -O3 -Wall -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5 -arch x86_64
# CFLAGS=/usr/local/lib/libgsl-10.5.a -O3 -Wall -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5 
CFLAGS=/usr/local/lib/libgsl.a -O3 -Wall
# CFLAGS=/usr/local/lib/libgsl.a -Wall -g

all:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) app.cpp -o app
