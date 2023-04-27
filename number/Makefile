CXX=clang++
CXXFLAGS=-g3 -O0 -std=c++20 -Wall -Weffc++ -pedantic \
	-pedantic-errors -Wextra -Waggregate-return -Wcast-align \
	-Wcast-qual -Wchar-subscripts -Wcomment -Wconversion \
	-Wdisabled-optimization -Wno-deprecated -Wno-unused-parameter \
	-Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security \
	-Wformat-y2k -Wimplicit -Wimport -Winit-self -Winline \
	-Winvalid-pch -Wlong-long -Wmissing-braces -Wmissing-field-initializers \
	-Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn \
	-Wpacked -Wparentheses -Wpointer-arith -Wredundant-decls -Wreturn-type \
	-Wsequence-point -Wshadow -Wsign-compare -Wstack-protector \
	-Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wswitch-default \
	-Wswitch-enum -Wtrigraphs -Wuninitialized \
	-Wunknown-pragmas -Wunreachable-code -Wunused \
	-Wunused-function -Wunused-label -Wunused-parameter \
	-Wunused-value -Wunused-variable -Wvariadic-macros \
	-Wvolatile-register-var -Wwrite-strings #-Wpadded will always warn about class Number

INC=src
OBJ=obj/math.o obj/type.o

all: main.out

main.out: obj/main.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $?

tests/%.out: tests/%.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $?

tests/%.o: tests/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $? -c -I src

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $? -c

clean:
	rm -f *.out obj/*.o tests/*.out

.PHONY: all clean
.SUFFIXES: # Disable built-in rules
