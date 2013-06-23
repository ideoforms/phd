# CPP=g++
CPP=clang++
INCLUDE=-I/usr/local/boost -I/usr/local/include -I.
SIM=sim/*.cpp sim/io/*.cpp sim/config/*.c
# CFLAGS=/usr/local/lib/libgsl.a -O3 -Wall -isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5 
# CFLAGS=/usr/local/lib/libgsl.a -O3 -Wall
CFLAGS=/usr/local/lib/libgsl.a 

all:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) app.cpp -o app

ca:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) app-ca.cpp -o app-ca

test:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) test-ca.cpp -o test-ca
