CC=clang++
CC_FLAGS=-Wall -Ofast
LIBS=-L/usr/include/boost -lboost_filesystem -lboost_system
EXEC=test.out
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

$(EXEC): $(OBJECTS)
	$(CC) $(LIBS) $(OBJECTS) -o $(EXEC)

%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
