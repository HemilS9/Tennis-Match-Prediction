CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic --std=c++17 -g -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG

predict.exe: predict.cpp
	$(CXX) $(CXXFLAGS) predict.cpp -o $@

clean:
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out
