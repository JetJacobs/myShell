default: main.cpp executor.cpp executor.hpp cmdParser.cpp cmdParser.hpp
	g++ -o myshell.o main.cpp executor.cpp executor.hpp cmdParser.cpp cmdParser.hpp

test: executor.cpp executor.hpp cmdParser.cpp cmdParser.hpp
	g++ -o test.o test.cpp cmdParser.cpp cmdParser.hpp executor.cpp executor.hpp

clean:
	rm -rvf *.o