CC       := clang++
WARNINGS := -Wall -Werror -Wnon-virtual-dtor -Wctor-dtor-privacy
CFLAGS   := -c -pipe -ggdb3 -std=c++11 $(shell sdl2-config --cflags) \
            $(WARNINGS) -Iinclude -Ilib/include -I/usr/local/include
LFLAGS   := $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer -lGL -lGLU \
            -lGLEW -ltmxparser
PROJDIRS := include src
SOURCES  := $(shell find $(PROJDIRS) -type f -name "*.cpp")
#HEADERS  := $(shell find $(PROJDIRS) -type f -name "*.hpp")
OBJECTS  := $(patsubst %.cpp, %.o, $(SOURCES))
DEPFILES := $(patsubst %.cpp, %.dep, $(SOURCES))
TARGET   := tile

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(LFLAGS) $^

clean:
	rm -f $(TARGET) $(OBJECTS) $(DEPFILES)
	find . -type f -iname "*~" -exec rm -f {} \;

-include $(DEPFILES)

%.o: %.cpp
	$(CC) -o $@ $(CFLAGS) -MMD -MP -MF $(patsubst %.cpp, %.dep, $<) $<

