CXX := clang++
CPPFLAGS := -I..
CXXFLAGS := -Wall -Wextra -Werror -pedantic-errors -std=c++11 -O2

DOMUS_DIR = ./
DOMUS = domus

all: $(DOMUS)

DEPS = *.h

% : %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -f $(DOMUS) *.o

