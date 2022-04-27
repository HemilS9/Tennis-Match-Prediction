CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic --std=c++11 -g -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG

prediction: predict.cpp
	$(CXX) $(CXXFLAGS) predict.cpp -o $@

clean:
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out
