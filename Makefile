CPP=clang++
INCLUDE=-I/usr/local/boost -I/usr/local/include -I.
SIM=app-utils.cpp sim/*.cpp sim/io/*.cpp sim/config/*.c
CFLAGS=/usr/local/lib/libgsl.a -O3 -Wall

all:
	gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C < sim/config/settings.ggo
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) app.cpp -o app

.PHONY : landscape-analysis
landscape-analysis:
	$(CPP) $(INCLUDE) $(SIM) $(CFLAGS) landscape-analysis.cpp -o landscape-analysis
