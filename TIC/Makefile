CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -I./googletest/googletest/include
LDFLAGS = -L./googletest/build/lib -lgtest

all: tic_app

tic_app: main.cpp tic.cpp tic.h
	$(CXX) $(CXXFLAGS) main.cpp tic.cpp -o tic_app

test: test_tic

test_tic: test_tic.cpp tic.cpp tic.h
	$(CXX) $(CXXFLAGS) test_tic.cpp tic.cpp $(LDFLAGS) -o test_tic

run_tests: test
	./test_tic

clean:
	rm -f tic_app test_tic