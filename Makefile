PROGRAM = tiuridar

TEST = testEditor
OBJECTS = build/cell.o build/tables.o build/line.o build/cell.o build/commands.o build/execute.o build/operators.o build/help.o build/graph.o
HEADERS = src/cell/cell.h	src/commands/commands.h src/execute/execute.h src/graph/graph.h src/help/help.h src/line/line.h src/operators/operators.h src/tables/tables.h
SOURCES = src/cell/cell.cpp src/commands/commands.cpp src/execute/execute.cpp src/graph/graph.cpp src/help/help.cpp src/line/line.cpp src/operators/operators.cpp src/tables/tables.cpp

CC = g++
CFLAGS = -std=c++17 -Wall -pedantic -Wextra -Wshadow -Wconversion -Wunreachable-code -g -Wno-long-long -O0 -ggdb

all: compile doc

doc: ./Doxyfile $(HEADERS)
	doxygen Doxyfile

test: $(OBJECTS)
	$(CC) $(CFLAGS) $^ tests/tests.cpp -o $(TEST)
	./$(TEST)
	rm -r $(TEST)

clean:
	rm -f -r doc
	rm -f -r build
	rm -f $(PROGRAM)

run: compile
	./$(PROGRAM)

compile: username

memcheck: compile
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROGRAM)

username: build/main
	cp $< $(PROGRAM)

build/main: $(OBJECTS) | objs
	$(CC) $(CFLAGS) $^ src/main.cpp -o $@

build/%.o: src/*/%.cpp
	$(CC) $(CFLAGS) $< -c -o $@

build/main.o: src/main.cpp | objs

build/table.o: src/tables/tables.cpp src/tables/tables.h | objs

build/line.o: src/line/line.cpp src/line/line.h | objs

build/cell.o: src/cell/cell.cpp src/cell/cell.h | objs

build/commands.o: src/commands/commands.cpp src/commands/commands.h | objs

build/execute.o: src/execute/execute.cpp src/execute/execute.h | objs

build/operators.o: src/operators/operators.cpp src/operators/operators.h | objs

build/help.o: src/help/help.cpp src/help/help.h | objs

build/graph.o: src/graph/graph.cpp src/graph/graph.h | objs

objs:
	mkdir -p build

.PHONY: clean