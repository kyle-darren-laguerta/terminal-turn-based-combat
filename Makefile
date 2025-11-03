CXX = g++
LDFLAGS = -lnlohmann_json_schema_validator -lncursesw

SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp, %.o, ${SRCS})

all: final.o
	./final.o

final.o: ${OBJS} a.o b.o
	@echo "Linking the objects and libraries..."
	@${CXX} $^ ${LDFLAGS} -o $@

%.o: %.cpp
	@echo "Compiling $<..."
	@${CXX} -c -Iheader/utils $< -o $@

clean:
	@rm -f *.o
	@echo "Removing $(wildcard *.o)..."
	@rm -f $(wildcard src/*.o)
	@echo "Removong $(wildcard src/*.o)..."
	@echo "Done removing all the object files"

ui: a.o b.o
	@${CXX} a.o b.o ${LDFLAGS} -o ui.o
	@rm -f a.o b.o

a.o:
	@echo "Compiling main.cpp..."
	@${CXX} -c main.cpp -o a.o

b.o:
	@echo "Compiling io.cpp..."
	@${CXX} -c -Iheader/utils src/utils/io.cpp -o b.o
