COMPILER = clang++ -O0 -g
CXXFLAGS = $(shell llvm-config-12 --cxxflags)
LDFLAGS = $(shell llvm-config-12 --ldflags --libs)
SOURCES = $(shell find src -name '*.cpp')
OBJECTS = $(SOURCES:.cpp=.o)
OUTPUT = hack

.PHONY: all clean

all: $(OUTPUT)

%.o: %.cpp
	$(COMPILER) $(CXXFLAGS) -c $< -o $@

$(OUTPUT): $(OBJECTS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(OUTPUT)

test: $(OUTPUT)
	./$(OUTPUT)
