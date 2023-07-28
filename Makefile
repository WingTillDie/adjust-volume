CXX := x86_64-w64-mingw32-g++
CXXFLAGS := -Wall
LDFLAGS := -static
LDLIBS := -lole32 -lstdc++

EXES := adjust-volume toggle-volume

.PHONY: all
all: $(EXES)

adjust-volume:

toggle-volume:

.PHONY: clean
clean:
	rm -f $(EXES)

.PHONY: run
run: adjust-volume
	./adjust-volume
