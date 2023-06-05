EXECUTABLE = predict

CXX = g++
CXXFLAGS = -Wall -Werror -pedantic --std=c++17 -g -fsanitize=address -fsanitize=undefined

SOURCES = $(wildcard *.cpp)

predict.exe: predict.cpp
	$(CXX) $(CXXFLAGS) predict.cpp -o $@

clean:
	rm -f $(EXECUTABLE)_debug 
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out

debug: CXXFLAGS += -g3 -DDEBUG
debug:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)_debug
