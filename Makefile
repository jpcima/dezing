CXXFLAGS = -O2 -g -Wall
LDFLAGS =

CXXFLAGS += `pkg-config libpcrecpp --cflags`
LDFLAGS += `pkg-config libpcrecpp --libs` -lgettextpo

all: dézing

clean:
	rm -f dézing dézing.o

dézing: dézing.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<
