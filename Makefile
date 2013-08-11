# CPP=g++
CPP=clang++
INCLUDE=-I/usr/local/boost -I/usr/local/include -I.
SIM=sim/*.cpp sim/io/*.cpp sim/config/*.c
# CFLAGS=/usr/local/lib/libgsl.a -O3 -Wall -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5 -arch x86_64
# CFLAGS=/usr/local/lib/libgsl-10.5.a -O3 -Wall -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5 
# CFLAGS=/usr/local/lib/libgsl.a -O3 -Wall
CFLAGS=/usr/local/lib/libgsl.a -Wall -g

all:
	make basic
	make ca
	make abm

basic:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) app.cpp -o app

ca:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) app-ca.cpp -o app-ca

abm:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) app-abm.cpp -o app-abm

test:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) test-ca.cpp -o test-ca

testabm:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) test-abm.cpp -o test-abm

testlandscape:
	g++ -lgsl -I. test-landscape.cpp sim/Landscape.cpp sim/Util.cpp -o test-landscape
