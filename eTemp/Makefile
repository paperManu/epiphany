CXX := g++
CXXFLAGS := -std=c++11

PREFIX := /usr/local

OBJECTS := main.o
PROGRAMS := eTemp

eTemp: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean install

clean:
	rm $(OBJECTS) $(PROGRAMS)

install: eTemp
	install -m 0755 eTemp $(prefix)/bin
