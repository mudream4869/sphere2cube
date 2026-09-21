CXX ?= g++
CXXFLAGS ?= -O2 -std=c++11 -pthread -Ithird_party/stb
LDFLAGS ?= -pthread

OBJS = main.o sphere2cube.o image.o

main: $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

main.o sphere2cube.o: sphere2cube.h image.h
image.o: image.h

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) main

.PHONY: clean
