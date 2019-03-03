CXX = gcc
src = $(wildcard src/*.c)
obj = $(src:.c=.o)

LDFLAGS = -Wall -Wextra -std=c14 -O3

cpm: $(obj)
	@mkdir -p bin
	$(CXX) -g -o bin/$@ $^ $(LDFLAGS) $(OSFLAGS)

clean:
	rm $(obj)
