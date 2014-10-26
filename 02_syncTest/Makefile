ESDK := $(EPIPHANY_HOME)
ELIBS := $(ESDK)/tools/host/lib
EINCS := $(ESDK)/tools/host/include
ELDF := $(ESDK)/bsps/current/legacy.ldf

CXX := g++
CXXFLAGS := -std=c++11 -O2 -g0
ECXX := e-g++
ECXXFLAGS := -O2 -g0 -T $(ELDF)

PROGRAMS := src/main src/e_main.srec

.PHONY: all clean

all: src/main src/e_main.srec

clean:
	rm src/main src/e_main.elf src/e_main.srec

src/main: src/main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ -I $(EINCS) -L $(ELIBS) -le-hal -lpthread

src/e_main.srec: src/e_main.cpp
	$(ECXX) $(ECXXFLAGS) $< -o src/e_main.elf -lelib
	e-objcopy --srec-forceS3 --output-target srec src/e_main.elf src/e_main.srec
