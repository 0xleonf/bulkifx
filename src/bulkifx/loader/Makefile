CXXFLAGS += -I/usr/include/opencv5 -I../../../include -fPIC
LDFLAGS += -lopencv_core -lopencv_imgcodecs -lstdc++

all: main_app

loader.o: loader.cpp
	$(CXX) $(CXXFLAGS) -c loader.cpp -o loader.o

main_app: main.c loader.o
	$(CC) main.c loader.o $(CXXFLAGS) $(LDFLAGS) -o main_app

run: main_app
	./main_app

clean:
	rm -f loader.o main_app

